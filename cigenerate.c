/*
  "Система обработки транзакций  с использованием 
  распределенных колоночных индексов"
  
  Модуль генерации колоночных индексов.
  Авторы: Иванова Е.В., Соколинский Л.Б.

*/
#include "cigenerate.h"
#include "ciload.h"

/*
	Генерация размера сегмента для разных типов распределения.
	Входные параметры:
	rows - количество кортежей во фрагменте,
	segment_num - номер сегмента.
	Выходной параметр:
	количество кортежей в текущем сегменте; значение 0 - размер не определен
*/
int gen_segmentsize(int table, int rows, int segment_num, segment_t *segment_table)
{
	int segmentsize = 0;
	int i;
	int error;
	
	// Вычисление размера сегмента с учетом вида распределения, указанного в SEGMENT_TETA
	if ((SEGMENT_TETA == 0.0) || (table == R)) {
		// Размер сегмента определяется как равномерное распределение rows кортежей на NUM_SEGMENTS частей
		segmentsize = (int)(rows / NUM_SEGMENTS);
		if ((rows % NUM_SEGMENTS) - segment_num >= 1)	// введение поправки для segmentsize 
			segmentsize++;								// в случае, если количество rows не делится 
														// без остатка на NUM_SEGMENTS
	}
	else {
		// Неравномерное распределение
		double p, H=0.0;
		int size = 0;
		int tmp;
		
		// Вычисляем H
		for(i=1; i<=NUM_SEGMENTS; i++) {
			H = H + pow(i,SEGMENT_TETA*(-1));
		}
		p = 1.0/(pow(segment_num+1,SEGMENT_TETA)*H); 	// Вероятность распределения кортежей в сегменте
		segmentsize = (int)round(rows * p); 			// Вычисляем количество кортежей в сегменте
		
		size = segmentsize;
		
		
		// Проверка на ошибку из-за округления
		if(segment_num == 0) {
			
			for(i=1; i<NUM_SEGMENTS; i++) {
				size += segment_table[i].size;
			}
			if(size != rows) {
				error = rows - size;
				segmentsize += error;
			}
		}
	}
	
	return segmentsize;
}

int gen_segment(int table, int segment_num, int size, int *ptr_table, int start_key)
{
	int i;
	int key, value;
	int num_values;
	
	key = start_key;
	
	if(table == R) {
		value = segment_domain[segment_num];
		for(i=0; i<size; i++) {
			ptr_table[db_index_key(size, i)] = key;
			ptr_table[db_index_value(size, i)] = value;
			key++;
			value++;
		}
	}
	if(table == S) {	
		for(i=0; i<size; i++) {
			num_values = segment_domain[segment_num+1] - segment_domain[segment_num];
			num_values = (int)(num_values*NVALUES/100);
			if(num_values <= 1)
				value = segment_domain[segment_num];
			else {
			    //srand(1);
				value = segment_domain[segment_num] + (int)rand() % num_values;
			}
			ptr_table[db_index_key(size, i)] = key;
			ptr_table[db_index_value(size, i)] = value;
			key++;
		}
		quickSort_ci(ptr_table, size, 0, size-1); // Сортировка колоночного индекса по значениям
	}
	
	return 0;
}

/*
	Сортировка пузырьковая
*/
int bubbleSort_ci(int *ci, int size)
{
	int i, j, tmp_key, tmp_val;
 
	for (i=size-1; i>0; i--) {
		for (j=0; j<i; j++) {
			if (ci[db_index_value(size, j)] > ci[db_index_value(size, j+1)]) {
				// Меняем кортежи местами
				tmp_key = ci[db_index_key(size, j)];
				tmp_val = ci[db_index_value(size, j)];
				ci[db_index_key(size, j)] = ci[db_index_key(size, j+1)];
				ci[db_index_value(size, j)] = ci[db_index_value(size, j+1)];
				ci[db_index_key(size, j+1)] = tmp_key;
				ci[db_index_value(size, j+1)] = tmp_val;
			}
		}
	}
	
	return 0;
}

/*
	Сортировка быстрая
*/
int quickSort_ci(int *ci, int size, int first, int last)
{
	int i=first, j=last; 		// поставить указатели на исходные места
	int tmp_key, tmp_val, p;

	p = ci[ db_index_value(size, (first + last)/2) ];		// центральный элемент

	// процедура разделения
	do {
		while ( ci[db_index_value(size, i)] < p )
			i++;
		while ( ci[db_index_value(size, j)] > p )
			j--;

		if (i <= j) {
			if(ci[db_index_value(size, i)] > ci[db_index_value(size, j)]) {
				// Меняем кортежи местами
				tmp_key = ci[db_index_key(size, j)];
				tmp_val = ci[db_index_value(size, j)];
				ci[db_index_key(size, j)] = ci[db_index_key(size, i)];
				ci[db_index_value(size, j)] = ci[db_index_value(size, i)];
				ci[db_index_key(size, i)] = tmp_key;
				ci[db_index_value(size, i)] = tmp_val;
			}
			i++; j--;
		}
	} while ( i<=j );


	// рекурсивные вызовы, если есть, что сортировать 
	if ( j > first )
		quickSort_ci(ci, size, first, j);
	if ( last > i )
		quickSort_ci(ci, size, i, last);
		return 0;
}

