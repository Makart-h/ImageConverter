#ifndef PNGDECODER_H_INCLUDED
#define PNGDECODER_H_INCLUDED
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "FourBytes.h"

#define HEADER_SIZE 8

bool PNG_Decode(FILE* png);
#endif // !PNGDECODER
