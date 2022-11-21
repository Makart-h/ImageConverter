#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

typedef uint8_t byte;
#define BYTE_SIZE 1
#define BYTE_COUNT 4

typedef struct {
	union {
		struct {
			byte byte1;
			byte byte2;
			byte byte3;
			byte byte4;
		};
		uint32_t int32Value;
	};
} FourBytes;

void FourB_ReadFromBuffer(FourBytes* byte4, byte* buffer, int offset, bool fromLeft);
void FourB_ReadFromFILE(FILE* png, FourBytes* buffer, bool fromLeftToRight);