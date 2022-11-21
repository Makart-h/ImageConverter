#include "IDAT.h"
#include <stdlib.h>
#include <string.h>

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
			errno_t error2 = memcpy_s(newData + first->DataSize - 1, otherSize, other->Data, otherSize);
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