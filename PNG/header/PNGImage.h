#pragma once
#include "Chunks/Critical/IHDR.h"
#include "Chunks/Critical/PLTE.h"
#include "Chunks/Critical/IDAT.h"
#include <stdbool.h>

typedef struct {
	IHDR* Header;
	PLTE* Palette;
	IDAT* Data;
	bool IsValid;
	byte PixelWidth;
} PNGImage;

PNGImage* PNGI_Get(IHDR* header, PLTE* palette, IDAT* compressedData);