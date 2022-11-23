#pragma once
#include <stdint.h>
#include "FourBytes.h"

enum ColorType
{
	Grayscale = 0,
	Truecolor = 2,
	IndexedColor = 3,
	GrayscaleAlpha = 4,
	TruecolorAlpha = 6
};

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
byte IHDR_GetPixelWidth(byte colorType);
