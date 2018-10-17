#ifndef __MEMORY__
#define __MEMORY__

#include "types.h"

void initMemory();

byte getMemoryTableEntry(bool used, bool shared, bool isLastByte, byte processId);

bool isMemoryUsed(byte* memoryTableEntry);
bool isMemoryShared(byte* memoryTableEntry);
bool isLastByte(byte* memoryTableEntry);
byte getProcessId(byte* memoryTableEntry);

uint32_t* getMemoryPointerFromMemoryTablePointer(byte* memoryTableEntry);
byte* getMemoryTablePointerForMemoryPointer(uint32_t* memory);

// size in bytes
void* pmalloc(int size, byte processId);
void* smalloc(int size);
void* malloc(int size);
void free(void* pointer);
void freeProcessMemory(byte processId);
#endif