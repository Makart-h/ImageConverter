#pragma once
#include "IHDR.h"
#include "PLTE.h"
#include "IDAT.h"
#include <stdbool.h>

typedef struct {
	IHDR* Header;
	PLTE* Palette;
	IDAT* Data;
	bool IsValid;
	byte PixelWidth;
} PNGImage;

PNGImage* PNGI_Get(IHDR* header, PLTE* palette, IDAT* compressedData);