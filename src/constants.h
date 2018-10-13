#ifndef __CONSTANTS__
#define __CONSTANTS__
#define STACK_START        0xBFFFFFFF
#define STACK_END          0xBF000000 // 16MB application stack
#define HEAP_END           0x89999999
#define HEAP_START         0x3F000000 // 2GB application heap

#define PROCESS_STACK_SIZE 0x8000     // 32KB stack size per process
#define PROCESS_COUNT      52         // 52 processes at once at most
#endif