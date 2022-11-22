#include "PNGImage.h"
#include <stdlib.h>

static void DecompressData(PNGImage* image);
static void ReverseFiltering(PNGImage* image);

PNGImage* PNGI_Get(IHDR* header, PLTE* palette, IDAT* compressedData)
{
	PNGImage* image = (PNGImage*)malloc(sizeof(PNGImage));
	if (image != NULL)
	{
		image->IsValid = false;
		if (header != NULL && compressedData != NULL)
		{
			image->Header = header;
			image->Palette = palette;
			image->Data = compressedData;
			image->PixelWidth = IHDR_GetPixelWidth(image->Header->ColorType);
			if (image->PixelWidth == -1)
				return image;
			DecompressData(image);
		}	
	}
	return image;
}

static void DecompressData(PNGImage* image)
{
	IDAT* decompressed = IDAT_Decompress(image->Data);
	if (decompressed != NULL)
	{
		free(image->Data);
		image->Data = decompressed;
		ReverseFiltering(image);
	}
}

static void ReverseFiltering(PNGImage* image)
{
	if (image->Header->FilterMethod != 0)
		return;

	// Reverse filtering
}