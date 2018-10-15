#ifndef __MEMORYSTRATEGIES__
#define __MEMORYSTRATEGIES__

#include "types.h"

typedef uint32_t* (*MemoryStrategy)(int);

extern int selectedMemoryStrategy;

uint32_t* findFirst(int blockSize);

MemoryStrategy getSelectedMemoryStrategy();

#endif