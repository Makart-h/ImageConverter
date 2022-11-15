#pragma once
#include <stdint.h>
#include "Byte4.h"

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

void PrintIHDR(IHDR* ihdr);
IHDR* GetIHDR(byte* data);
