/*
  "Система обработки транзакций  с использованием 
  распределенных колоночных индексов"
  
  Модуль завершения работы с колоночными индексами.
  Авторы: Иванова Е.В., Соколинский Л.Б.

*/
#include "finalize.h"

int finalize()
{
	int segment_num;
	
	// Печать результата
	print_result();
	
	// Освобождение памяти
	free(segment_domain);
	for(segment_num=0; segment_num<NUM_SEGMENTS; segment_num++) {
		free(segmentR[segment_num].ptr);
		free(segmentS[segment_num].ptr);
		free(segmentRes[segment_num].ptr);
	}
	free(segmentR);
	free(segmentS);
	free(segmentRes);
	
	return 0;
}