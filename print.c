/*
  "Система обработки транзакций  с использованием 
  распределенных колоночных индексов"
  
  Модуль вывода параметров эксперимента и результата.
  Авторы: Иванова Е.В., Соколинский Л.Б.

*/
#include "print.h"

/*	
	Печать параметров программы
*/
int print_header()
{
	printf("\n\tNumber of fragments: %d\n", NUM_FRAGMENTS);
	printf("\tNumber of fragment's segments: %d\n", NUM_SEGMENTS);
	printf("\tNumber of Xeon Phi threads: %d\n", NUM_THREADS);
	printf("\tDomain values: %d..%d\n", MIN_VAL_DOMAIN, MAX_VAL_DOMAIN);
	printf("\tR size = %d, S size = %d\n", NUM_ROWS_R, NUM_ROWS_S);
	printf("\t(segment distribution = %0.2f)\n", SEGMENT_TETA);
	printf("\tNumber of repeats: %d\n", NREPEAT);
	printf("\tNumber of S values: %d\n", NVALUES);
	printf("\tMODE:");
	
	#ifdef USE_FILE_DB // Режим с использoванием существующей в файле базы данных
		printf(" Use exist database.");
	#endif
	#ifdef CREATE_FILE_DB // Режим с созданием файла БД для использования в режиме USE_FILE_DB
		printf(" Create database file.");
	#endif
	#ifdef USE_COMPRESS // Режим с использoванием сжатия данных для передачи на Intel Xeon Phi
		printf(" Use compression (level %d)", COMPRESS_LEVEL);
	#endif
		
	printf("\n\t------------------------------------\n\n");

	return 0;
}

/*	
	Печать результата программы
*/
int print_result()
{	
	long int res_size = 0;
	int segment_num;
	
	#ifdef USE_DEBUG_PRINT
		int i;
		//int segment_num;
		int *int_ptr;
		
		printf("INFO: Result = {");
		for(segment_num=0; segment_num<NUM_SEGMENTS; segment_num++) {
			int_ptr = (int *)segmentRes[segment_num].ptr;
			printf("\n\t\t");
			for(i=0; i<segmentRes[segment_num].size; i++)
				printf(" (%d, %d)", int_ptr[res_index_key1(i)], int_ptr[res_index_key2(i)]);
		}
		printf("\n\t  }\n");
	#endif
	
	printf("\n\n\t---------------------- \n");
	printf("\texecution time = %lf \n", EXEC_TIME);
	printf("\tcalculation time = %lf \n", CALC_TIME/(NREPEAT*NUM_SEGMENTS));
	printf("\tuncompress data time = %lf \n", UNCOMPRESS_TIME/(NREPEAT*NUM_SEGMENTS));
	printf("\tgenerate index = %lf\n", GENERATION_TIME);
	printf("\tnumber of operations = %lld\n\n", FLOPS/NREPEAT);
	
	for(segment_num=0; segment_num<NUM_SEGMENTS; segment_num++)
		res_size += segmentRes[segment_num].size;
	printf("\tsize of result = %ld\n\n", res_size);
		
	return 0;
}