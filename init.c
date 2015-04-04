/*
  "Система обработки транзакций  с использованием 
  распределенных колоночных индексов"
  
  Модуль инициализации работы с колоночными индексами.
  Включает в себя получение данных
  и загрузку их в оперативную память.
  Авторы: Иванова Е.В., Соколинский Л.Б.

*/
#include "init.h"
#include "cilocate.h"

int parameters_control();

/*
	Инициализация работы с колоночными индексами.
	Функция обрабатывает параметры запуска программы.
	Формат запуска программы:
		slave <NUM_THREADS> <COMPRESS_LEVEL> <NUM_SEGMENTS> <NVALUES> <SEGMENT_TETA> <NUM_ROWS_R> <NUM_ROWS_S>
*/

int init(int argc, char *argv[])
{
	double start_t, end_t;
	char filenameR[15], filenameS[15];
	
	// Инициализация параметров
	// ------------------------	
	if (argc > 1) {
		/* Parse command line arguments */
		NUM_THREADS = atoi(argv[1]);
		if (NUM_THREADS <= 0) {
			printf("ERROR: Invalid NUM_THREADS.\n");
			return -1;
		}
		if (argc > 2)
			COMPRESS_LEVEL = atoi(argv[2]);
			
		if (argc > 3)
			NUM_SEGMENTS = atoi(argv[3]);
			
		if (argc > 4)
			NVALUES = atoi(argv[4]);
		
		if (argc > 5)
			SEGMENT_TETA = atof(argv[5]);
		
		if (argc > 6)
			NUM_ROWS_R = atoi(argv[6]);
		if (argc > 7)
			NUM_ROWS_S = atoi(argv[7]);
	}	
	#if defined(USE_FILE_DB) || defined(CREATE_FILE_DB)
		sprintf(filenameR, "%sR%d.txt", DIR, FRAGMENT_NUM);	// Имя файла с текущим фрагментом базы данных
		sprintf(filenameS, "%sS%d.txt", DIR, FRAGMENT_NUM);	// Имя файла с текущим фрагментом базы данных
	#endif
	
	#ifdef USE_FILE_DB
		R_DB = fopen(filenameR, "r");
		S_DB = fopen(filenameS, "r");
		if(!R_DB || !S_DB) {
			printf("ERROR: Cannot open file '%s' or '%s'!\n", filenameR, filenameS);
			return 0;
		}
		fscanf(R_DB, "%d", &NUM_ROWS_R);
		fscanf(S_DB, "%d", &NUM_ROWS_S);
	#endif
	#ifdef CREATE_FILE_DB
		//NUM_SEGMENTS = NUM_ROWS_R;
		R_DB = fopen(filenameR, "w");
		S_DB = fopen(filenameS, "w");
		if(!R_DB || !S_DB) {
			printf("ERROR: Cannot create file '%s' or '%s'!\n", filenameR, filenameS);
			return 0;
		}
		fprintf(R_DB, "%d\n", NUM_ROWS_R);
		fprintf(S_DB, "%d\n", NUM_ROWS_S);
	#endif
	
	MIN_VAL_DOMAIN = 1;				// Нижняя граница домена
	MAX_VAL_DOMAIN = NUM_ROWS_R;	// Верхняя граница домена
	NUM_ROWS_Res = NUM_ROWS_S;
	
	srand(100);
	
	// Проверка согласованности параметров
	// ---------------------------------------------
	parameters_control();
	
	// Печать параметров
	// ------------------------------
	print_header();
	
	// Создание таблицы сегментации домена в текущем фрагменте
	// ------------------------------------------------
	segment_domain = memalign(ALIGN, (NUM_SEGMENTS+1)*sizeof(int));
	if(segment_domain == NULL) {
		printf("ERROR: Could not allocate memmory for segment_domain!\n");
		return -1;
	}
	create_segment_table(segment_domain);
	
	// Получение колоночных индексов и размещение их в памяти
	// ---------------------------------------------
	start_t = omp_get_wtime();	// Замер времени
	ci_locate();				// Получение колоночных индексов и размещение их в памяти
	end_t = omp_get_wtime();	// Замер времени
	GENERATION_TIME = end_t - start_t;
	
	// Вычисление размера сжатой и несжатой БД
	//----------------------------------------
	int segment_num;
	long int byte_sizeR=0, byte_sizeS=0, byte_compress_sizeR=0, byte_compress_sizeS=0;
	for(segment_num=0; segment_num<NUM_SEGMENTS; segment_num++) {
		byte_sizeR += segmentR[segment_num].size;
		byte_sizeS += segmentS[segment_num].size;
		byte_compress_sizeR += segmentR[segment_num].compress_size;
		byte_compress_sizeS += segmentS[segment_num].compress_size;
	}
	byte_sizeR = byte_sizeR*2*sizeof(int);
	byte_sizeS = byte_sizeS*2*sizeof(int);
	printf("INFO: R size = %ld bytes, S size = %ld bytes,\n\tR compressed size = %.4lf Mb (%ld bytes),\n\tS compressed size = %.4lf Mb (%ld bytes)\n",
	byte_sizeR, byte_sizeS, (double)byte_compress_sizeR/Mb, byte_compress_sizeR, (double)byte_compress_sizeS/Mb, byte_compress_sizeS);
	
	#if defined(USE_FILE_DB) || defined(CREATE_FILE_DB)
		fclose(R_DB);
		fclose(S_DB);
	#endif
	
	return 0;
}

/*
	Функция проверки согласованности параметров программы.
*/
int parameters_control()
{
	// Количество кортежей в R должно быть не больше, чем в S
	assert(NUM_ROWS_R <= NUM_ROWS_S);
	
	// Оценка достаточности памяти для размещения данных на Xeon Phi
	assert(2*(NUM_ROWS_R+NUM_ROWS_S+NUM_ROWS_S)/NUM_FRAGMENTS <= MEMORY_SIZE_PHI);
	
	// Количество сегментов в домене не должно превышать количества кортежей в R
	assert(NUM_SEGMENTS <= NUM_ROWS_R);
	
	// Количество сегментов должно быть кратно количеству кортежей в R
	assert(NUM_ROWS_R % NUM_SEGMENTS == 0);
	
	// Количество значений в домене должно совпадать с количеством кортежей в R
	assert((MAX_VAL_DOMAIN - MIN_VAL_DOMAIN + 1) == NUM_ROWS_R);
	
	return 0;
}
