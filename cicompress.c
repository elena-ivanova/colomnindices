/*
  "Система обработки транзакций  с использованием 
  распределенных колоночных индексов"
  
  Модуль сжатия и распаковки данных.
  Авторы: Иванова Е.В., Соколинский Л.Б.

*/
#include "cicompress.h"
#include "zlib/zlib.h"

int segment_compress(int *source, int size, char *compress_source, long int *compress_size)
{	
	int err;
	
	uLong sourceLen = (uLongf)size;
	uLongf destLen = (uLongf)(*compress_size);
	Bytef *byte_source;
	Bytef *byte_dest;
	
	byte_source = (Bytef *)source;
	byte_dest = (Bytef *)compress_source;
	
	err = compress2(byte_dest, &destLen, byte_source, sourceLen, COMPRESS_LEVEL);
	
	if(err != Z_OK) {
		printf("ERROR: Error compress!\n");
		if(err == Z_BUF_ERROR)
			printf("ERROR: The buffer was not large enough to hold the uncompressed data.\n");
		if(err == Z_MEM_ERROR)
			printf("ERROR: Insufficient memory.\n");
		if(err == Z_STREAM_ERROR)
			printf("ERROR: The compressed data (referenced by source) was corrupted.\n");
	}

	*compress_size = (long int)destLen;
	
	return 0;
}

int segment_uncompress(char *source, long int compress_size, int *tmp_segment, long int *uncompress_size)
{
	int err;
	Bytef *byte_source, *byte_dest;
	uLong sourceLen = (uLongf)compress_size;
	uLongf destLen = (uLongf)(*uncompress_size);
	
	byte_source = (Bytef *)source;
	byte_dest = (Bytef *)tmp_segment;
	
	err = uncompress(byte_dest, &destLen, byte_source, sourceLen);
	if(err != Z_OK) {
		printf("Error uncompress!\n");
		if(err == Z_BUF_ERROR)
			printf("The buffer was not large enough to hold the uncompressed data.\n");
		if(err == Z_MEM_ERROR)
			printf("Insufficient memory.\n");
		if(err == Z_DATA_ERROR)
			printf("The compressed data (referenced by source) was corrupted.\n");
	}
	
	*uncompress_size = (long int)destLen;

	return 0;
}
