#ifndef __KEYBOARD__
#define __KEYBOARD__
#include "types.h"

#define STATUS_REGISTER 0x64
#define DATA_REGISTER   0x60

void installKeyboard();
char getLastCharacter();

char* scan(int maxLength, char* pointer);
#endif