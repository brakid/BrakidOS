#ifndef __MEMORYSTRATEGIES__
#define __MEMORYSTRATEGIES__

#include "types.h"

typedef uint32_t* (*MemoryStrategy)(int);

extern int selected;

uint32_t* findFirst(int size);

MemoryStrategy getSelectedMemoryStrategy();

#endif