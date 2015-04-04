/*
  "Система обработки транзакций  с использованием 
  распределенных колоночных индексов"
  
  Модуль загрузки колоночных индексов из файла.
  Авторы: Иванова Е.В., Соколинский Л.Б.

*/
#include "ciload.h"
#include "db.h"

/*
	Загрузка сегмента segment_num колоночного индекса таблицы table.
	Входные данные:
	size - размер загружаемого сегмента (в кортежах)
	ptr - указатель на область памяти, где нужно разместить сегмент
*/
int load_segment(int size, int *ptr, FILE *hf)
{
	int key, value, i;
	
	for(i=0; i<size; i++) {
		fscanf(hf, "%d %d", &key, &value);
		ptr[db_index_key(size, i)] = key;
		ptr[db_index_value(size, i)] = value;
	}
	
	return 0;
}

/*
	Запись сегмента в файл
*/
int save_segment(int size, int *ptr, FILE *hf)
{
	int i;
		
	for(i=0; i<size; i++)
		fprintf(hf, "%d %d\n", ptr[db_index_key(size, i)], ptr[db_index_value(size, i)]);
			
	return 0;
}
