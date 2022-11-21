#include "IHDR.h"
#include <stdlib.h>

static void ReadCompressionMethod(IHDR* ihdr, byte* data, int* offset);
static void ReadFilterMethod(IHDR* ihdr, byte* data, int* offset);
static void ReadInterlaceMethod(IHDR* ihdr, byte* data, int* offset);
static void ReadBitDepthAndColorType(IHDR* ihdr, byte* data, int* offset);
static bool ValidateBitDepth(byte colorType, byte bitDepth);
static void ReadSize(IHDR* ihdr, byte* data, int* offset);
static uint32_t ReadInt32(byte* data, int* offset);

IHDR* IHDR_Get(byte* data)
{
	IHDR* ihdr = malloc(sizeof(IHDR));
	if (ihdr != NULL)
	{
		int offset = 0;		
		ReadSize(ihdr, data, &offset);
		ReadBitDepthAndColorType(ihdr, data, &offset);		
		ReadCompressionMethod(ihdr, data, &offset);
		ReadFilterMethod(ihdr, data, &offset);
		ReadInterlaceMethod(ihdr, data, &offset);
	}
	return ihdr;
}

static void ReadCompressionMethod(IHDR* ihdr, byte* data, int* offset)
{
	ihdr->CompressionMethod = *(data + (*offset)++);
	if (ihdr->CompressionMethod != 0)
	{
		ihdr->IsValid = false;
		printf("Not a valid compression method!");
	}
}

static void ReadFilterMethod(IHDR* ihdr, byte* data, int* offset)
{
	ihdr->FilterMethod = *(data + (*offset)++);
	if (ihdr->FilterMethod != 0)
	{
		ihdr->IsValid = false;
		printf("Not a valid filter method!");
	}
}

static void ReadInterlaceMethod(IHDR* ihdr, byte* data, int* offset)
{
	ihdr->InterlaceMethod = *(data + (*offset)++);
	if (ihdr->InterlaceMethod != 0 && ihdr->InterlaceMethod != 1)
	{
		ihdr->IsValid = false;
		printf("Not a valid interlace method!");
	}
}

static void ReadBitDepthAndColorType(IHDR* ihdr, byte* data, int* offset)
{	
	ihdr->BitDepth = *(data + (*offset)++);
	ihdr->ColorType = *(data + (*offset)++);

	const byte validColorTypes[] = { 0, 2, 3, 4, 6 };
	bool isColorValid = false;
	for (int i = 0; i < sizeof validColorTypes; i++)
	{
		if (ihdr->ColorType == validColorTypes[i])
		{
			isColorValid = true;
			break;
		}
	}
	if (isColorValid)
	{
		if (!ValidateBitDepth(ihdr->ColorType, ihdr->BitDepth))
			ihdr->IsValid = false;
	}
	else
	{
		ihdr->IsValid = false;
		printf("Not a valid color type!");
	}
}

static bool ValidateBitDepth(byte colorType, byte bitDepth)
{
	byte validValues[] = { 1, 2, 4, 8, 16 };
	byte firstIndex = 0, lastIndex = sizeof validValues - 1;
	bool isBitDepthValid = false;
	switch (colorType)
	{
	case 3:
		lastIndex--;
		break;
	default:
		firstIndex = lastIndex - 1;
		break;
	}
	for (int i = firstIndex; i <= lastIndex; i++)
	{
		if (bitDepth == validValues[i])
		{
			isBitDepthValid = true;
			break;
		}
	}
	if (isBitDepthValid == false)
		printf("Error bit depth [%d] and color type [%d] combination is prohibited!", bitDepth, colorType);
	return isBitDepthValid;
}

static void ReadSize(IHDR* ihdr, byte* data, int* offset)
{
	ihdr->Width = ReadInt32(data, offset);
	ihdr->Height = ReadInt32(data, offset);
	if (ihdr->Width == 0)
	{
		ihdr->IsValid = false;
		printf("Error! Width=%d", ihdr->Width);
	}
	if (ihdr->Height == 0)
	{
		ihdr->IsValid = false;
		printf("Error! Height=%d", ihdr->Height);
	}
}

static uint32_t ReadInt32(byte* data, int* offset)
{
	FourBytes fourB;
	FourB_ReadFromBuffer(&fourB, data, *offset, false);
	*offset += sizeof(FourBytes);
	return fourB.int32Value;
}

void IHDR_Print(IHDR* ihdr)
{
	printf("Width: %d\n", ihdr->Width);
	printf("Height: %d\n", ihdr->Height);
	printf("Bit depth: %d\n", ihdr->BitDepth);
	printf("Color type: %d\n", ihdr->ColorType);
	printf("Compression method: %d\n", ihdr->CompressionMethod);
	printf("Filter method: %d\n", ihdr->FilterMethod);
	printf("Interlace method: %d\n", ihdr->InterlaceMethod);
}