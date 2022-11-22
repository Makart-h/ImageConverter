#include "Filtering.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

static byte FiltSub(byte x, byte a);
static byte FiltUp(byte x, byte b);
static byte FiltAverage(byte x, byte a, byte b);
static byte FiltPaeth(byte x, byte a, byte b, byte c);
static byte ReconSub(byte x, byte a, byte rA);
static byte ReconUp(byte x, byte b, byte rB);
static byte ReconAverage(byte x, byte a, byte b, byte rA, byte rB);
static byte ReconPaeth(byte x, byte a, byte b, byte c, byte rA, byte rB, byte rC);
static byte PaethPredictor(byte a, byte b, byte c);

byte* Filtering_ReconstructData(byte* filteredData, size_t imageWidth, size_t imageHeight, byte pixelWidth, byte bitDepth)
{
	byte* data = (byte*)malloc(imageWidth * imageHeight * pixelWidth);
	if (data != NULL)
	{
		if (bitDepth == BYTE_BIT_COUNT || bitDepth == BYTE_BIT_COUNT * 2)
		{
			byte* current = filteredData;
			for (int i = 0; i < imageHeight; i++)
			{
				byte filterType = *current;
				current++;
				for (int j = 0; j < imageWidth * (pixelWidth * bitDepth / BYTE_BIT_COUNT); j++)
				{
					byte a = j == 0 ? 0 : *(current - pixelWidth);
					byte b = i == 0 ? 0 : *(current - imageWidth - 1);
					byte c = i == 0 || j == 0 ? 0 : *(current - imageWidth - 1 - pixelWidth);
					byte rA = j == 0 ? 0 : *(data - pixelWidth);
					byte rB = i == 0 ? 0 : *(data - imageWidth);
					byte rC = i == 0 || j == 0 ? 0 : *(data - imageWidth - pixelWidth);
					switch (filterType)
					{
					case Sub:
						*(data++) = ReconSub(*current, a, rA);
						break;
					case Up:
						*(data++) = ReconUp(*current, b, rB);
						break;
					case Average:
						*(data++) = ReconAverage(*current, a, b, rA, rB);
						break;
					case Paeth:
						*(data++) = ReconPaeth(*current, a, b, c, rA, rB, rC);
						break;
					}
					current++;
				}
			}
		}
		else
		{
			// Handle bit depths smaller than byte.
		}
	}
	return data;
}

static byte FiltSub(byte x, byte a)
{
	return x - a;
}

static byte FiltUp(byte x, byte b)
{
	return x - b;
}

static byte FiltAverage(byte x, byte a, byte b)
{
	return x - (((uint16_t)a + (uint16_t)b) >> 1);
}

static byte FiltPaeth(byte x, byte a, byte b, byte c)
{
	return x - PaethPredictor(a, b, c);
}
static byte PaethPredictor(byte a, byte b, byte c)
{
	uint16_t p = a + b - c;
	uint16_t pa = abs(p - a);
	uint16_t pb = abs(p - b);
	uint16_t pc = abs(p - c);

	if (pa <= pb && pa <= pc)
		return a;
	else if (pb <= pc)
		return b;
	else
		return c;
}
static byte ReconSub(byte x, byte a, byte rA)
{
	FiltSub(x, a) + rA;
}
static byte ReconUp(byte x, byte b, byte rB)
{
	FiltUp(x, b) + rB;
}
static byte ReconAverage(byte x, byte a, byte b, byte rA, byte rB)
{
	FiltAverage(x, a, b) + (((uint16_t)rA + (uint16_t)rB) >> 1);
}
static byte ReconPaeth(byte x, byte a, byte b, byte c, byte rA, byte rB, byte rC)
{
	FiltPaeth(x, a, b, c) + PaethPredictor(rA, rB, rC);
}