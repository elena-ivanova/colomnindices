/*
  "Система обработки транзакций  с использованием 
  распределенных колоночных индексов"
  
  Модуль исполнения запросов.
  Авторы: Иванова Е.В., Соколинский Л.Б.

*/
#include "execute.h"
#include "cicompress.h"

int execute()
{
	int segment_num;			// Счетчик сегментов
	int i;
	int *tmp_segmentR, *tmp_segmentS;
	double start_t, end_t;
	double start_calc_t, end_calc_t;
	
	#ifdef USE_COMPRESS // Режим с использованием сжатия данных
		start_t = omp_get_wtime();	// Замер времени
		long int max_byte_segm_size = (long int)(MAX_SEGMENT_S_SIZE*sizeof(int)*2.4 + 12);
		long int uncompress_size = max_byte_segm_size;
	
		// Выделение памяти для временного хранения сегментов
		tmp_segmentR = memalign(ALIGN, 2*max_byte_segm_size*sizeof(int));
		tmp_segmentS = memalign(ALIGN, 2*max_byte_segm_size*sizeof(int));
		if((tmp_segmentR == NULL) || (tmp_segmentS == NULL)) {
			printf("ERROR: could not allocate memmory for tmp_segment R or S!\n");
		}
		end_t = omp_get_wtime();	// Замер времени
		UNCOMPRESS_TIME += (end_t - start_t);
	#endif
	
	// Распределение сегментов по нитям
	#pragma omp for schedule(dynamic, 1)
	for(segment_num=0; segment_num<NUM_SEGMENTS; segment_num++) {
		#ifdef USE_COMPRESS // Режим с использвоанием сжатия данных
			#ifdef USE_UNCOMPRESS_TIME
				//if(omp_get_thread_num() == 0)
					start_t = omp_get_wtime();	// Замер времени
			#endif
			
			uncompress_size = max_byte_segm_size;
			segment_uncompress(segmentR[segment_num].ptr, segmentR[segment_num].compress_size, tmp_segmentR, &uncompress_size);
			uncompress_size = max_byte_segm_size;
			segment_uncompress(segmentS[segment_num].ptr, segmentS[segment_num].compress_size, tmp_segmentS, &uncompress_size);
			
			#ifdef USE_UNCOMPRESS_TIME
				//if(omp_get_thread_num() == 0) {
					end_t = omp_get_wtime();	// Замер времени
				#pragma omp atomic
					UNCOMPRESS_TIME += (end_t - start_t);
				//}
			#endif
		#else
			tmp_segmentR = (int *)segmentR[segment_num].ptr;
			tmp_segmentS = (int *)segmentS[segment_num].ptr;
		#endif
		
		#ifdef USE_CALC_TIME
			//if(omp_get_thread_num() == 0)
				start_calc_t = omp_get_wtime();	// Замер времени
		#endif
		
		//merge_join(tmp_segmentR, segmentR[segment_num].size, tmp_segmentS, segmentS[segment_num].size, (int *)segmentRes[segment_num].ptr, &segmentRes[segment_num].size);
		teta_join(tmp_segmentR, segmentR[segment_num].size, tmp_segmentS, segmentS[segment_num].size, (int *)segmentRes[segment_num].ptr, &segmentRes[segment_num].size);
		
		#ifdef USE_CALC_TIME
			//if(omp_get_thread_num() == 0) {
				end_calc_t = omp_get_wtime();	// Замер времени
			#pragma omp atomic
				CALC_TIME += (end_calc_t - start_calc_t);
			//}
		#endif
	}
	
	#ifdef USE_COMPRESS // Режим с использвоанием сжатия данных
		free(tmp_segmentR);
		free(tmp_segmentS);
	#endif
	
	return 0;
}


int teta_join(int *Rbuf, int sizeR, int *Sbuf, int sizeS, int *Resbuf, int *sizeRes)
{
	int startR, endR, startS, endS;
	int iR, iS, i;
	int cnt = 0;
	
	startR = 0;
	endR = sizeR;
	startS = 0;
	endS = sizeS;
	
	for(iR=startR; iR<endR; iR++) {
		for(iS=startS; iS<endS; iS++) {
			if(Rbuf[db_index_value(sizeR, iR)] < Sbuf[db_index_value(sizeS, iS)]) {
				Resbuf[res_index_key1(cnt)] = Rbuf[db_index_key(sizeR, iR)];
				Resbuf[res_index_key2(cnt)] = Sbuf[db_index_key(sizeS, iS)];
				cnt++;
				
				for(i=iS+1; i<endS; i++) {
					Resbuf[res_index_key1(cnt)] = Rbuf[db_index_key(sizeR, iR)];
					Resbuf[res_index_key2(cnt)] = Sbuf[db_index_key(sizeS, i)];
					cnt++;

				}
				//cnt += endS-iS+1;
				startS = iS;
				break;
			}
		}
	}
	*sizeRes = cnt;
	
	return 0;
}

// Соединение слиянием
int merge_join(int *Rbuf, int sizeR, int *Sbuf, int sizeS, int *Resbuf, int *sizeRes)
{
	int startR, endR, startS, endS;
	int iR, iS;
	int cnt = 0;
	
	startR = 0;
	endR = sizeR;
	startS = 0;
	endS = sizeS;
	
	for(iR=startR; iR<endR; iR++) {
		#ifdef USE_FLOPS
			#pragma omp atomic
			FLOPS++;
			printf("+1\n");
		#endif
		for(iS=startS; iS<endS; iS++) {
			#ifdef USE_FLOPS
				#pragma omp atomic
				FLOPS++;
				printf("+1\n");
			#endif
			if(Rbuf[db_index_value(sizeR, iR)] == Sbuf[db_index_value(sizeS, iS)]) {
				Resbuf[res_index_key1(cnt)] = Rbuf[db_index_key(sizeR, iR)];
				Resbuf[res_index_key2(cnt)] = Sbuf[db_index_key(sizeS, iS)];
				cnt++;
				#ifdef USE_FLOPS
					#pragma omp atomic
					FLOPS += 4;
					printf("+4\n");
				#endif
			}
			else {
				if(Sbuf[db_index_value(sizeS, iS)] < Rbuf[db_index_value(sizeR, iR)]) {
					iS++;
					#ifdef USE_FLOPS
						#pragma omp atomic
						FLOPS += 3;
						printf("+3\n");
					#endif
				}
				else {
					startS = iS;
					#ifdef USE_FLOPS
						#pragma omp atomic
						FLOPS += 2;
						printf("+2\n");
					#endif
					break;
				}
			}
		}
	}
	*sizeRes = cnt;
	
	return 0;
}