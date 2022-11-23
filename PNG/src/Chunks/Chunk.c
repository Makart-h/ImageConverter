#include <stdlib.h>
#include "Chunks/Chunk.h"
#include "Decoder.h"

#define BYTE_VALUES_COUNT 256
#define BITS_COUNT 8

static void CreateCRCTable();
static uint32_t UpdateCRC(uint32_t initializedCRC, byte* buffer, uint32_t bufferSize);
static uint32_t CalculateCRC(byte* buffer, uint32_t bufferSize);
static void ValidateChunkData(Chunk* chunk);
static byte* ConcatTypeAndData(Chunk* chunk, size_t* outBufferSize);

uint32_t crcTable[BYTE_VALUES_COUNT];
bool crcTableCreated = false;

bool Chunk_CompareType(Chunk* chunk, const char* const string)
{
    for (int i = 0; i < TYPE_BYTE_COUNT; ++i)
    {
        if (*(chunk->Type + i) != *(string + i))
            return false;
    }
    return true;
}

Chunk* Chunk_ReadData(FILE* png)
{
    Chunk* chunk = malloc(sizeof(Chunk));
    if (chunk != NULL)
    {
        FourBytes fourB;
        FourB_ReadFromFILE(png, &fourB, false);
        chunk->Length = fourB.int32Value;
        FourB_ReadFromFILE(png, &fourB, true);
        *(uint32_t*)chunk->Type = fourB.int32Value;
        if (chunk->Length > 0)
        {
            chunk->Data = (uint8_t*)malloc(chunk->Length);
            if (chunk->Data != NULL)
                fread_s(chunk->Data, chunk->Length, BYTE_SIZE, chunk->Length, png);
        }
        else
        {
            chunk->Data = NULL;
        }
        FourB_ReadFromFILE(png, &fourB, false);
        chunk->CRC = fourB.int32Value;
        ValidateChunkData(chunk);
    }
    return chunk;
}

static void ValidateChunkData(Chunk* chunk)
{
    size_t crcBufferSize = 0;
    byte* crcBuffer = ConcatTypeAndData(chunk, &crcBufferSize);
    if (crcBuffer != NULL)
    {      
        uint32_t calculated = CalculateCRC(crcBuffer, crcBufferSize);
        chunk->IsValid = calculated == chunk->CRC;
        free(crcBuffer);
    } 
}

static byte* ConcatTypeAndData(Chunk* chunk, size_t* outBufferSize)
{
    *outBufferSize = (size_t)(chunk->Length) + TYPE_BYTE_COUNT;
    byte* buffer = (byte*)malloc(*outBufferSize);
    if (buffer != NULL)
    {
        for (int i = 0, j = 0; i < *outBufferSize; ++i)
        {
            if (i < TYPE_BYTE_COUNT)
                *(buffer + i) = chunk->Type[i];
            else
            {
                *(buffer + i) = chunk->Data[j++];
            }
        }
    }
    return buffer;
}

static uint32_t CalculateCRC(byte* buffer, uint32_t bufferSize)
{
    return UpdateCRC(0xffffffffL, buffer, bufferSize) ^ 0xffffffffL;
}

static uint32_t UpdateCRC(uint32_t initializedCRC, byte* buffer, uint32_t bufferSize)
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

static void CreateCRCTable()
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