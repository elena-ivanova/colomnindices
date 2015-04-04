#ifndef _CONFIG_H
#define _CONFIG_H
/*
  "Система обработки транзакций  с использованием 
  распределенных колоночных индексов"
  
  Файл с общими параметрами
  Авторы: Иванова Е.В., Соколинский Л.Б.
  
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Режимы программы
//#define USE_DEBUG_PRINT (1)		// Режим с отладочной печатью
//#define USE_FILE_DB (1)		// Режим с использованием существующего файла базы данных
								// Имя файла определяется в функции get_filename
//#define CREATE_FILE_DB (1)	// Режим с созданием файла БД для использования в режиме USE_FILE_DB
#define USE_COMPRESS (1)		// Режим с использвоанием сжатия данных для передачи на Intel Xeon Phi

#define USE_CALC_TIME (1)
#define USE_UNCOMPRESS_TIME (1)
//#define USE_FLOPS (1)

extern int NUM_FRAGMENTS;		// Количество фрагментов
extern int FRAGMENT_NUM;		// Номер текущего фрагмента
extern int NUM_SEGMENTS;		// Количество сегментов во фрагменте
extern int NUM_THREADS;			// Количество нитей для запуска на сопроцессоре

extern int NUM_ROWS_R;			// Количество строк в таблице с первичным ключом R в текущем фраменте
extern int NUM_ROWS_S;			// Количество строк в таблице с внешним ключом S в текущем фраменте
extern int NUM_ROWS_Res;		// Количество строк в таблице с результатом в текущем фраменте

extern int MIN_VAL_DOMAIN;		// Нижняя граница домена в текущем фраменте
extern int MAX_VAL_DOMAIN;		// Верхняя граница домена в текущем фраменте

extern float SEGMENT_TETA;		// Вид распределения значений в таблице S по cегментам.
								// Коэффициент перекоса, изменяется от 0 до 1. Коэффициент 0.5 соответствует правилу "45-20".
								// Коэффициент 0.68 соответствует правилу "70-30". Коэффициент 0.2 соответствует правилу "25-15".
								// Коэффициент 0.86 соответствует правилу "80-20". Коэффициент 0 - равномерное распределение.

extern int *segment_domain; 	// Таблица сегментации домена в текущем фрагменте,
								// содержит значениями из домена, обозначающими границы 
								// сегментов в текущем фрагменте.
								// Пример: таблица segment_domain = {1,5,8} обозначает, что в текущем фрагменте два сегмента, 
								// границы первого сегмента от 1 до 4, второго - от 5 до 7 (включительно).

extern const int MEMORY_SIZE_PHI;	// Объем оперативной памяти сопроцессора (в целых числах),
									// доступной для размещения данных, определено экспериментально

extern int ALIGN;				// Значение, по которому будет производиться выравнивание данных

extern double GENERATION_TIME;	// Время загрузки данных при инициализации
extern double EXEC_TIME;		// Время выполнения запроса
extern double CALC_TIME;		// Время вычислений реляционной операции
extern double UNCOMPRESS_TIME;	// Время распаковки данных для выполнения реляционной операции

#define R (0)
#define S (1)

#define Mb (1048576)

typedef struct {
	int size;
	long int compress_size;
	char *ptr;
} segment_t;

extern segment_t *segmentR;	// Таблицы сегментации для	R
extern segment_t *segmentS;	// Таблицы сегментации для	S
extern segment_t *segmentRes;	// Таблицы сегментации для	результата операции

extern char DIR[5];

extern int MAX_SEGMENT_R_SIZE;
extern int MAX_SEGMENT_S_SIZE;

extern int COMPRESS_LEVEL;

extern int NREPEAT;			// Количество повторений эксперимента.

extern int NVALUES;			// Количество различных значений R.B в сегменте (в %)

extern FILE *R_DB;			// Указатель на фрагмент файла с базой данных R
extern FILE *S_DB;			// Указатель на фрагмент файла с базой данных S

extern long long int FLOPS;		// Количество операций

#endif