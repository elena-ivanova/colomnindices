/*
  "Система обработки транзакций  с использованием 
  распределенных колоночных индексов"
  
  Модуль получения колоночных индексов и размещение их в памяти.
  Авторы: Иванова Е.В., Соколинский Л.Б.

*/
#include "cilocate.h"
#include "cigenerate.h"
#include "ciload.h"


int ci_locate()
{
	int segment_num, i;
	
	
	// Выделение памяти для таблиц сегментации
	// ---------------------------------------
	segmentR = memalign(ALIGN, NUM_SEGMENTS*sizeof(segment_t));
	segmentS = memalign(ALIGN, NUM_SEGMENTS*sizeof(segment_t));
	segmentRes = memalign(ALIGN, NUM_SEGMENTS*sizeof(segment_t));
	if((segmentR == NULL) || (segmentS == NULL) || (segmentRes == NULL)) {
		printf("ERROR: could not allocate memmory for segment tables!\n");
		return -1;
	}
	
	
	// Заполнение размеров в таблицах сегментации
	// ------------------------------------------
	for(segment_num=NUM_SEGMENTS-1; segment_num>=0; segment_num--) {
		segmentR[segment_num].size = gen_segmentsize(R, NUM_ROWS_R, segment_num, segmentR);
		segmentS[segment_num].size = gen_segmentsize(S, NUM_ROWS_S, segment_num, segmentS);
		segmentRes[segment_num].size = segmentS[segment_num].size*segmentR[segment_num].size;
	}
	
	#ifdef USE_DEBUG_PRINT
		printf("INFO: segmentR = {");
		for(segment_num=0; segment_num<NUM_SEGMENTS; segment_num++)
			printf(" %d", segmentR[segment_num].size);
		printf(" }\n");
		printf("INFO: segmentS = {");
		for(segment_num=0; segment_num<NUM_SEGMENTS; segment_num++)
			printf(" %d", segmentS[segment_num].size);
		printf(" }\n");
	#endif
	
	
	// Определение максимального размера сегмента
	// ------------------------------------------
	MAX_SEGMENT_R_SIZE = 0;
	MAX_SEGMENT_S_SIZE = 0;
	for(segment_num=0; segment_num<NUM_SEGMENTS; segment_num++) {
		if (segmentR[segment_num].size > MAX_SEGMENT_R_SIZE)
			MAX_SEGMENT_R_SIZE = segmentR[segment_num].size;
		if (segmentS[segment_num].size > MAX_SEGMENT_S_SIZE)
			MAX_SEGMENT_S_SIZE = segmentS[segment_num].size;
	}
	
	
	// Загрузка сегментов
	// ------------------
	int *tmp_segment;
	long int max_byte_segm_size = (long int)(MAX_SEGMENT_S_SIZE*sizeof(int)*2.4 + 12);
	
	// Выделение памяти для временного хранения сегментов
	tmp_segment = memalign(ALIGN, 2*max_byte_segm_size*sizeof(int));
	
	// Загрузка сегментов R
	#ifdef USE_DEBUG_PRINT
		printf("INFO: R = {\n");
	#endif
	locate_segments(R, segmentR, max_byte_segm_size, tmp_segment);
	#ifdef USE_DEBUG_PRINT
		printf("\t  }\n");
	#endif
	
	// Загрузка сегментов S
	#ifdef USE_DEBUG_PRINT
		printf("INFO: S = {\n");
	#endif
	locate_segments(S, segmentS, max_byte_segm_size, tmp_segment);
	#ifdef USE_DEBUG_PRINT
		printf("\t  }\n");
	#endif
	
	free(tmp_segment);
	
	
	// Выделение памяти для таблицы с результатом
	// ------------------------------------------
	for(segment_num=0; segment_num<NUM_SEGMENTS; segment_num++) {
		segmentRes[segment_num].ptr = memalign(ALIGN, 2*segmentRes[segment_num].size*sizeof(int));
		if(segmentRes[segment_num].ptr == NULL) {
			printf("ERROR: could not allocate memmory for %dth segment in Res!\n", segment_num);
			return -1;
		}
	}
	
	return 0;
}

int locate_segments(int table, segment_t *segment_table, long int size_ptr, int *ptr)
{
	int start_key, i;
	int segment_num;
	char *compress_ptr;
	
	compress_ptr = memalign(ALIGN, 2*size_ptr*sizeof(int));
	
	for(segment_num=0; segment_num<NUM_SEGMENTS; segment_num++) {
		// Заполнение сегмента
		#ifdef USE_FILE_DB
		if (table == R)
			load_segment(segment_table[segment_num].size, ptr, R_DB);
		if (table == S)
			load_segment(segment_table[segment_num].size, ptr, S_DB);
		#else
			if(segment_num == 0) {
				start_key = 0;
			}
			else {
				start_key = start_key + segment_table[segment_num-1].size;
			}
			gen_segment(table, segment_num, segment_table[segment_num].size, ptr, start_key);
			#ifdef CREATE_FILE_DB
				if (table == R)
					save_segment(segment_table[segment_num].size, ptr, R_DB);
				if (table == S)
					save_segment(segment_table[segment_num].size, ptr, S_DB);
			#endif
		#endif
		
		// Вывод сегмента на печать
		#ifdef USE_DEBUG_PRINT
			printf("\t\t");
			for(i=0; i<segment_table[segment_num].size; i++)
				printf(" (%d, %d)", ptr[db_index_key(segment_table[segment_num].size, i)], ptr[db_index_value(segment_table[segment_num].size, i)]);
			printf("\n");
		#endif
		
		// Сжатие сегмента
		#ifdef USE_COMPRESS
			segment_table[segment_num].compress_size = size_ptr;
			segment_compress(ptr, 2*sizeof(int)*segment_table[segment_num].size, compress_ptr, &segment_table[segment_num].compress_size);
		#else
			segment_table[segment_num].compress_size = 2*sizeof(int)*segment_table[segment_num].size;
		#endif
		
		// Выделение памяти для постоянного хранения сегмента
		segment_table[segment_num].ptr = (char *)memalign(ALIGN, segment_table[segment_num].compress_size+1);
		
		if(segment_table[segment_num].ptr == NULL) {
			printf("ERROR: could not allocate memmory for %dth segment!\n", segment_num);
			return -1;
		}
		
		// Копирование сегмента в место постоянного хранения
		#ifdef USE_COMPRESS
			str_copy(compress_ptr, segment_table[segment_num].ptr, segment_table[segment_num].compress_size+1);
		#else
			str_copy(ptr, segment_table[segment_num].ptr, segment_table[segment_num].compress_size+1);
		#endif
		
		// Вывод сжатого сегмента на печать
		#ifdef USE_DEBUG_PRINT
			printf("\t\t segment was compressed to %ld bytes (%s)\n", segment_table[segment_num].compress_size, segment_table[segment_num].ptr);
		#endif
	}
	
	free(compress_ptr);
	
	return 0;
}

int str_copy(char *source, char *dest, long int size)
{
	int i;
	for(i=0; i<size; i++)
		dest[i] = source[i];
	return 0;
};
		
