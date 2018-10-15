#ifndef __PROCESSMANAGER__
#define __PROCESSMANAGER__

#define CS_VALUE 0x8
#define EFLAGS_VALUE 0x202 //interupts enabled

#define KERNEL_PROCESS_ID 0

#define saveContext() \
        asm("pusha;"  \
        "push ds;"    \
        "push es;"    \
        "push fs;"    \
        "push gs;") 

#define restoreContext() \
        asm("pop gs;"    \
        "pop fs;"        \
        "pop es;"        \
        "pop ds;"        \
        "popa;"          \
        "add esp, 0x1c")

#include "types.h"
#include "process.h"

extern Process* currentProcess;

byte getNewProcessId();
byte getCurrentProcessId();
Process* getCurrentProcess();

void startProcessManager();
void killProcess();
void startProcess();
void yield();

#endif