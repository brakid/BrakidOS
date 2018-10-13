#ifndef __CONSTANTS__
#define __CONSTANTS__
#define HEAP_END           0x3FFFFFFF // 1024-128MB heap
#define HEAP_START         0x08000000 // 128MB code + kernel stack
#define MEMORY_TABLE_END   0x0B333333 // end of memory table
#define PROCESS_STACK_SIZE 0x4000     // 14KB stack size per process
#define BYTE_RATIO         4          // one record in memory table represents 4 bytes
#define BYTES_PER_RECORD   1          // 1 byte to keep flags for memory
// structure of memory table:
// 1 bit: used unused flag
// 1 bit: shared memory
// 1 bit: last byte of a chunk
// 5 bit: process id, kernel: process id 0
#endif