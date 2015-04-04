/*
  "Система обработки транзакций  с использованием 
  распределенных колоночных индексов"
  
  Модуль сегментирования домена
  Авторы: Иванова Е.В., Соколинский Л.Б.

*/

#include "domain.h"

/*	
	Разбиение домена на сегменты для текущего фрагмента. Разбиение производится равномерно 
	от MIN_VAL_DOMAIN до MAX_VAL_DOMAIN на NUM_SEGMENTS сегментов в каждом фрагменте.
	table - возвращаемый параметр, таблица со значениями из домена, обозначающими границы 
	сегментов в текущем фрагменте.
	Пример: таблица table = {1,5,8} обозначает, что в текущем фрагменте два сегмента, 
	границы первого сегмента от 1 до 4, второго - от 5 до 7 (включительно).
*/
int create_segment_table(int *table)
{
	int size_fragment;		// Количество значений домена в текущем фрагменте
	int size_segment;		// Количество значений в сегментах текущего фрагмента
	int i;
	
	// Определение количества значений домена в текущем фрагменте
	size_fragment = MAX_VAL_DOMAIN - MIN_VAL_DOMAIN + 1;
		
	// Определение размера сегментов во фрагменте и заполнение таблицы table
	table[0] = MIN_VAL_DOMAIN;
	for(i=1; i<=NUM_SEGMENTS; i++) {
		size_segment = size_fragment / NUM_SEGMENTS;
		if ((size_fragment % NUM_SEGMENTS) - (i-1) >= 1)// введение поправки для size_segment
			size_segment++;								// в случае, если размер фрагмента не делится 
														// без остатка на количество сегментов
		table[i] = table[i-1] + size_segment;
	}
	
	/* Отладочная печать */
	#ifdef USE_DEBUG_PRINT
		printf("INFO: segment domain table = {");
		for(i=0; i<=NUM_SEGMENTS; i++){
			printf(" %d", table[i]);
		}
		printf(" }\n");
	#endif
	
	return 0;
}