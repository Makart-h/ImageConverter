#include "PLTE.h"
#include <stdlib.h>
#include <string.h>

PLTE* PLTE_Get(Chunk* chunk)
{
	if (chunk == NULL)
		return NULL;

	uint32_t length = chunk->Length;

	if (length > 0 && length % COLOR_CHANNELS_COUNT == 0)
	{
		PLTE* plte = (PLTE*)malloc(sizeof(PLTE));
		if (plte != NULL)
		{
			plte->Count = length / COLOR_CHANNELS_COUNT;
			plte->Entries = (RGB*)malloc(length);
			if (plte->Entries != NULL)
			{			
				errno_t error = memcpy_s(plte->Entries, length, chunk->Data, length);
				if (error != 0)
				{
					printf("Error occured reading PLTE data!");
					free(plte->Entries);
					free(plte);
					return NULL;
				}
			}
		}
		return plte;
	}
	else
	{
		printf("Not a valid PLTE data!");
		return NULL;
	}
}

void PLTE_Free(PLTE* plte)
{
	if (plte != NULL)
		free(plte->Entries);
	free(plte);
}