#ifndef __KEYBOARD__
#define __KEYBOARD__
#include "types.h"

extern byte keyboardUs[128];

void installKeyboard();
char getLastCharacter();
#endif