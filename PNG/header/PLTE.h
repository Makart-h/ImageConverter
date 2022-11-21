#pragma once
#include <stdint.h>
#include "Chunk.h"
#include "ImageData/header/RGB.h"

#define COLOR_CHANNELS_COUNT 3

typedef struct {
	uint16_t Count;
	RGB* Entries;
} PLTE;

PLTE* PLTE_Get(Chunk* chunk);
void PLTE_Free(PLTE* plte);