#ifndef _EXECUTE_H
#define _EXECUTE_H
/*
  "Система обработки транзакций  с использованием 
  распределенных колоночных индексов"
  
  Модуль исполнения запросов.
  Авторы: Иванова Е.В., Соколинский Л.Б.
  
*/

#include "common.h"

int execute();

int merge_join(int *Rbuf, int sizeR, int *Sbuf, int sizeS, int *Resbuf, int *sizeRes);

#endif