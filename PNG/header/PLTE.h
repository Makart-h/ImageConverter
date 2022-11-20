#pragma once
#include <stdint.h>
#include "Chunk.h"
#include "ImageData/header/RGB.h"

#define COLOR_CHANNELS_COUNT 3

typedef struct {
	uint16_t Count;
	RGB* Entries;
} PLTE;

PLTE* GetPLTE(Chunk* chunk);
void FreePLTE(PLTE* plte);