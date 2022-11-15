#include "Byte4.h"

void ReadByte4FromFILE(FILE* png, Byte4* buffer, bool fromLeftToRight)
{
	size_t count = sizeof * buffer / BYTE_SIZE;
	if (fromLeftToRight)
	{
		fread_s(buffer, sizeof * buffer, BYTE_SIZE, count, png);
	}
	else
	{
		byte* byte = buffer;
		for (int i = count - 1; i >= 0; i--)
		{
			fread_s(byte + i, BYTE_SIZE, BYTE_SIZE, BYTE_SIZE, png);
		}
	}
}

void ReadByte4FromBuffer(Byte4* byte4, byte* buffer, int offset, bool fromLeft)
{
	byte* p = byte4;
	if (fromLeft)
	{
		for (int i = 0; i < 4; i++)
		{
			*(p++) = *(buffer + offset + i);
		}
	}
	else
	{
		for (int i = 3; i >= 0; i--) {
			*(p++) = *(buffer + offset + i);
		}
	}
}