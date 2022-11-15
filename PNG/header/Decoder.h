#ifndef PNGDECODER_H_INCLUDED
#define PNGDECODER_H_INCLUDED
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "Byte4.h"

#define HEADER_SIZE 8

bool DecodePNG(FILE* png);
#endif // !PNGDECODER
