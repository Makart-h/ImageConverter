#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define TYPE_SIZE 4
typedef struct {
	uint32_t Length;
	uint8_t Type[TYPE_SIZE];
	uint8_t* Data;
	uint32_t CRC;
	bool IsValid;
} Chunk;

Chunk* ReadChunkData(FILE* png);
bool CompareChunkType(Chunk* chunk, const char* const string);
