#pragma once
#include <stdint.h>
#include "Chunk.h"

typedef struct
{
	size_t DataSize;
	uint8_t* Data;
} IDAT;

IDAT* IDAT_Get(Chunk* chunk);
IDAT* IDAT_Concat(IDAT* first, IDAT* other);
IDAT* IDAT_Decompress(IDAT* compressed);