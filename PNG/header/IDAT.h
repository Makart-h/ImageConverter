#pragma once
#include <stdint.h>
#include "Chunk.h"

typedef struct
{
	size_t DataSize;
	uint8_t* Data;
} IDAT;

IDAT* GetIDAT(Chunk* chunk);
IDAT* ConcatIDATs(IDAT* first, IDAT* other);