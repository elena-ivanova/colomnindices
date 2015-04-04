#ifndef _CIGENERATE_H
#define _CIGENERATE_H
/*
  "Система обработки транзакций  с использованием 
  распределенных колоночных индексов"
  
  Модуль генерации колоночных индексов.
  Авторы: Иванова Е.В., Соколинский Л.Б.
  
*/

#include "common.h"

/*
	Генерация размера сегмента в зависимости от типа распределения и таблицы table.
	Входные параметры:
	rows - количество кортежей во фрагменте,
	segment_num - номер сегмента.
	Выходной параметр:
	количество кортежей в текущем сегменте; значение 0 - размер не определен
*/
int gen_segmentsize(int table, int rows, int segment_num, segment_t *segment_table);

int gen_segment(int table, int segment_num, int size, int *ptr_table, int start_key);

#endif