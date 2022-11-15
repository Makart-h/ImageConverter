#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

typedef uint8_t byte;
#define BYTE_SIZE 1

typedef struct Byte4 {
	union {
		struct {
			byte byte1;
			byte byte2;
			byte byte3;
			byte byte4;
		};
		uint32_t int32Value;
	};
} Byte4;

void ReadByte4FromBuffer(Byte4* byte4, byte* buffer, int offset, bool fromLeft);
void ReadByte4FromFILE(FILE* png, Byte4* buffer, bool fromLeftToRight);