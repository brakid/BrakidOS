#ifndef __UTILS__
#define __UTILS__

#include "types.h"

char digitToChar(byte digit);

byte* memcpy(byte* dest, const byte* src, uint32_t count);
byte* memset(byte* dest, byte val, uint32_t count);
uint16_t* memset(uint16_t* dest, uint16_t val, uint32_t count);
uint32_t* memset(uint32_t* dest, uint32_t val, uint32_t count);

void disableInterupts();
void enableInterupts();
#endif