#ifndef _CICOMPRESS_H
#define _CICOMPRESS_H
/*
  "Система обработки транзакций  с использованием 
  распределенных колоночных индексов"
  
  Модуль сжатия и распаковки данных.
  Авторы: Иванова Е.В., Соколинский Л.Б.
  
*/

#include "common.h"

int segment_compress(int *source, int size, char *compress_source, long int *compress_size);

int segment_uncompress(char *source, long int compress_size, int *tmp_segment, long int *uncompress_size);

#endif