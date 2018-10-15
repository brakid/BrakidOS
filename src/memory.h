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

uint32_t* pmalloc(int size, byte processId);
uint32_t* smalloc(int size);
uint32_t* malloc(int size);
void free(uint32_t* pointer);
#endif