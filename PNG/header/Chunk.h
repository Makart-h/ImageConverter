#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define TYPE_BYTE_COUNT 4
typedef struct {
	uint32_t Length;
	uint8_t Type[TYPE_BYTE_COUNT];
	uint8_t* Data;
	uint32_t CRC;
	bool IsValid;
} Chunk;

Chunk* Chunk_ReadData(FILE* png);
bool Chunk_CompareType(Chunk* chunk, const char* const string);
