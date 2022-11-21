#pragma once
#include <stdint.h>
#include "FourBytes.h"

typedef struct {
	uint32_t Width;
	uint32_t Height;
	byte BitDepth;
	byte ColorType;
	byte CompressionMethod;
	byte FilterMethod;
	byte InterlaceMethod;
	bool IsValid;
} IHDR;

void IHDR_Print(IHDR* ihdr);
IHDR* IHDR_Get(byte* data);
