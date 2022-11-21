#include "IDAT.h"
#include <stdlib.h>
#include <string.h>
#include "zlib/zlib.h"

#define EXPECTED_COMPRESSION_RATIO 5

IDAT* IDAT_Get(Chunk* chunk)
{
	if (chunk == NULL)
		return NULL;

	size_t size = chunk->Length;
	IDAT* idat = (IDAT*)malloc(sizeof(IDAT));
	if (idat != NULL)
	{
		idat->DataSize = size;
		idat->Data = (uint8_t*)malloc(size);
		if (idat->Data != NULL)
		{
			errno_t error = memcpy_s(idat->Data, size, chunk->Data, size);
			if (error != 0)
			{
				printf("Error occured during reading the data!");
				free(idat->Data);
				free(idat);
				return NULL;
			}
		}
	}
	return idat;
}

IDAT* IDAT_Concat(IDAT* first, IDAT* other)
{
	if (first != NULL && other != NULL)
	{		
		size_t newSize = first->DataSize + other->DataSize;
		uint8_t* newData = (uint8_t*)malloc(newSize);
		if (newData != NULL)
		{
			errno_t error1 = memcpy_s(newData, first->DataSize, first->Data, first->DataSize);
			size_t otherSize = other->DataSize;
			errno_t error2 = memcpy_s(newData + first->DataSize, otherSize, other->Data, otherSize);
			if (error1 != 0 || error2 != 0)
			{
				return NULL;
			}
			else
			{
				IDAT* newIDAT = (IDAT*)malloc(sizeof(IDAT));
				if (newIDAT != NULL)
				{
					newIDAT->Data = newData;
					newIDAT->DataSize = newSize;
					free(first->Data);
					free(other->Data);
					first = NULL;
					other = NULL;
				}
				return newIDAT;
			}
		}
	}
	return NULL;
}

IDAT* IDAT_Decompress(IDAT* compressed)
{
	IDAT* decompressed = (IDAT*)malloc(sizeof(IDAT));
	if (decompressed != NULL)
	{
		decompressed->DataSize = compressed->DataSize * EXPECTED_COMPRESSION_RATIO;
		decompressed->Data = (uint8_t*)malloc(decompressed->DataSize);
		if (decompressed->Data != NULL)
		{
			int outcome = uncompress(decompressed->Data, &decompressed->DataSize, compressed->Data, compressed->DataSize);
			switch (outcome)
			{
			case Z_OK:
				uint8_t* old = decompressed->Data;
				decompressed->Data = realloc(decompressed->Data, decompressed->DataSize);
				if (decompressed->Data == NULL)
					decompressed->Data = old;
				return decompressed;
			case Z_MEM_ERROR:
				printf("Not enough memory!\n");
				break;
			case Z_BUF_ERROR:
				printf("Not enough room in the output buffer!\n");
				break;
			case Z_DATA_ERROR:
				printf("The input data was corrupted!\n");
				break;
			}
		}
	}
	return NULL;
}