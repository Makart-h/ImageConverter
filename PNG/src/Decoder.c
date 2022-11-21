#include <stdlib.h>
#include <stdbool.h>
#include "Decoder.h"
#include "Chunk.h"
#include "FourBytes.h"
#include "IHDR.h"
#include "PLTE.h"
#include "IDAT.h"
#include "PNGImage.h"

bool PNG_Decode(FILE* png);
bool DecodeHeader(FILE* png);
bool HandleChunk(Chunk* chunk);

IDAT* concatenatedIDAT = NULL;

bool PNG_Decode(FILE* png)
{
	if (png == NULL)
		return false;

	if (!DecodeHeader(png))
		return false;

	bool shouldContinue = true;
	while (shouldContinue)
	{
		Chunk* chunk = Chunk_ReadData(png);
		if (chunk != NULL && chunk->IsValid)
		{
			shouldContinue = HandleChunk(chunk);
			free(chunk);
		}
		else
		{
			printf("Not a valid chunk data! CRC mismatch!");
			return false;
		}
	}
	return true;
}

bool DecodeHeader(FILE* png)
{
	// These are always the first 8 bytes of a PNG datastream.
	const byte signature[HEADER_SIZE] = { 137, 80, 78, 71, 13, 10, 26, 10 };
	byte header[HEADER_SIZE];
	fread_s(header, HEADER_SIZE, BYTE_SIZE, HEADER_SIZE, png);
	for (int i = 0; i < HEADER_SIZE; ++i) {
		if (header[i] != signature[i])
			return false;
	}
	return true;
}

bool HandleChunk(Chunk* chunk)
{
	bool shouldContinue = false;
	if (Chunk_CompareType(chunk, "IHDR"))
	{
		IHDR* ihdr = IHDR_Get(chunk->Data);
		if (ihdr != NULL && ihdr->IsValid)
		{
			IHDR_Print(ihdr);
			free(ihdr);
			shouldContinue = true;
		}
	}
	else if (Chunk_CompareType(chunk, "PLTE"))
	{
		printf("Handling PLTE!\n");
		PLTE* plte = PLTE_Get(chunk);
		if (plte != NULL)
		{
			for (int i = 0; i < plte->Count; ++i)
			{
				RGB* entry = (plte->Entries + i);
				printf("[%3d] R->%3d, G->%3d, B->%3d \n", i, entry->Red, entry->Green, entry->Blue);
			}
		}
		PLTE_Free(plte);
		shouldContinue = true;
	}
	else if (Chunk_CompareType(chunk, "IDAT"))
	{
		printf("Handling IDAT!\n");
		IDAT* idat = IDAT_Get(chunk);
		if (idat != NULL)
		{
			if (concatenatedIDAT != NULL)
			{
				size_t size = concatenatedIDAT->DataSize;
				concatenatedIDAT = IDAT_Concat(concatenatedIDAT, idat);
				if (concatenatedIDAT != NULL)
				{
					printf("previous mega size: %lld, current mega size: %lld\n", size, concatenatedIDAT->DataSize);
				}
				else
				{
					printf("Concat didn't work :(\n");
				}
			}
			else
			{
				printf("First IDAT size: %lld\n", idat->DataSize);
				concatenatedIDAT = idat;
			}
		}
		shouldContinue = true;
	}
	else if (Chunk_CompareType(chunk, "IEND"))
	{
		printf("Handling IEND!\n");
		PNGImage* image = PNGI_Get(ihdr, plte, concatenatedIDAT);
		shouldContinue = false;
	}
	else
	{
		printf("Handling ancillary chunk!\n");
		for (int i = 0; i < 4; i++)
			printf("%c", *(chunk->Type + i));
		printf("\n");
		shouldContinue = true;
	}
	free(chunk->Data);
	return shouldContinue;
}