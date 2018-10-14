#ifndef __CONSTANTS__
#define __CONSTANTS__

#include "types.h"

#define ALLOCATION_SIZE   sizeof(uint32_t) // one record in memory table represents 4 bytes
#define TABLE_RECORD_SIZE sizeof(byte)     // 1 byte to keep flags for memory

#define HEAP_END           0x3FFFFFFF // 1024-128MB heap
#define HEAP_START         0x08000000 // 128MB code + kernel stack
#define MEMORY_TABLE_END   ((HEAP_END - HEAP_START) / (TABLE_RECORD_SIZE + ALLOCATION_SIZE)) + HEAP_START// end of memory table
#define PROCESS_STACK_SIZE 0x4000     // 16KB stack size per process
// structure of memory table:
// 1 bit: used unused flag
// 1 bit: shared memory
// 1 bit: last byte of a chunk
// 5 bit: process id, kernel: process id 0
#endif