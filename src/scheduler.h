#ifndef __SCHEDULER__
#define __SCHEDULER__

#include "types.h"

#define KERNEL_PROCESS_ID 0

#define saveContext() \
        asm("pusha;"  \
        "push ds;"    \
        "push es;"    \
        "push fs;"    \
        "push gs") 

#define restoreContext() \
        asm("pop gs;"    \
        "pop fs;"        \
        "pop es;"        \
        "pop ds;"        \
        "popa;"          \
        "add esp, 0x1c")

byte getCurrentProcessId();
void setCurrentProcessId(byte processId);
void yield();

#endif