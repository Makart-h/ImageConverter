#pragma once
#include <stdint.h>

typedef uint8_t byte;
#define BYTE_BIT_COUNT 8

enum MethodZeroTypes
{
	None, Sub, Up, Average, Paeth
};

byte* Filtering_ReconstructData(byte* filteredData , size_t imageWidth, size_t imageHeight, byte pixelWidth, byte bitDepth);