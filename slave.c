/*
  "Система обработки транзакций  с использованием 
  распределенных колоночных индексов"
  
  Главный модуль
  Авторы: Иванова Е.В., Соколинский Л.Б.
*/

#include "common.h"
#include "init.h"
#include "execute.h"
#include "finalize.h"
#include "cicompress.h"


int main(int argc, char *argv[])
{
	double start_t, end_t;
	int i;
	
	init(argc, argv);			// Инициализация работы с колоночными индексами
	
	start_t = omp_get_wtime();	// Замер времени
	for (i=0; i<NREPEAT; i++) {
		#pragma omp parallel num_threads(NUM_THREADS) //reduction(+:FLOPS)
		{
			execute();
		}
	}
	end_t = omp_get_wtime();	// Замер времени
	EXEC_TIME = (end_t - start_t)/NREPEAT;
	
	finalize();		// Завершение работы с колоночными индексами
	
	return 0;
}
