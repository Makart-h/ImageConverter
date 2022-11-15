#include <stdlib.h>
#include "Chunk.h"
#include "Decoder.h"

#define BYTE_VALUES_COUNT 256
#define BITS_COUNT 8

void CreateCRCTable();
uint32_t UpdateCRC(uint32_t initializedCRC, byte* buffer, uint32_t bufferSize);
uint32_t CalculateCRC(byte* buffer, uint32_t bufferSize);
void ValidateChunkData(Chunk* chunk);

uint32_t crcTable[BYTE_VALUES_COUNT];
bool crcTableCreated = false;

bool CompareChunkType(Chunk* chunk, const char* const string)
{
    for (int i = 0; i < TYPE_SIZE; ++i)
    {
        if (*(chunk->Type + i) != *(string + i))
            return false;
    }
    return true;
}

Chunk* ReadChunkData(FILE* png)
{
    Chunk* chunk = malloc(sizeof(Chunk));
    if (chunk != NULL)
    {
        Byte4 byte4;
        ReadByte4FromFILE(png, &byte4, false);
        chunk->Length = byte4.int32Value;
        ReadByte4FromFILE(png, &byte4, true);
        *(uint32_t*)chunk->Type = byte4.int32Value;
        if (chunk->Length > 0)
        {
            chunk->Data = (uint8_t*)malloc(chunk->Length);
            if (chunk->Data != NULL)
                fread_s(chunk->Data, chunk->Length, BYTE_SIZE, chunk->Length, png);
        }
        ReadByte4FromFILE(png, &byte4, false);
        chunk->CRC = byte4.int32Value;
        ValidateChunkData(chunk);
    }
    return chunk;
}

void ValidateChunkData(Chunk* chunk)
{
    size_t size = (size_t)(chunk->Length) + 4;
    uint8_t* buffer = (uint8_t*)malloc(size);
    for (int i = 0, j = 0; i < size; ++i)
    {
        if (i < TYPE_SIZE)
            *(buffer + i) = chunk->Type[i];
        else
        {
            *(buffer + i) = chunk->Data[j++];
        }
    }
    uint32_t calculated = CalculateCRC(buffer, size);
    chunk->IsValid = calculated == chunk->CRC;
    free(buffer);
}

uint32_t CalculateCRC(byte* buffer, uint32_t bufferSize)
{
    return UpdateCRC(0xffffffffL, buffer, bufferSize) ^ 0xffffffffL;
}

uint32_t UpdateCRC(uint32_t initializedCRC, byte* buffer, uint32_t bufferSize)
{
    uint32_t crc = initializedCRC;

    if (!crcTableCreated)
        CreateCRCTable();

    for (uint32_t i = 0; i < bufferSize; i++)
    {
        crc = crcTable[(crc ^ buffer[i]) & 0xff] ^ (crc >> 8);
    }

    return crc;
}

void CreateCRCTable()
{
    uint32_t c;
    uint32_t n, k;

    for (n = 0; n < BYTE_VALUES_COUNT; n++)
    {
        c = n;
        for (k = 0; k < BITS_COUNT; k++)
        {
            if (c & 1)
                c = 0xedb88320L ^ (c >> 1);
            else
                c = c >> 1;
        }
        crcTable[n] = c;
    }
    crcTableCreated = true;
}