#ifndef __REGS__
#define __REGS__
#include "types.h"

/* This defines what the stack looks like after an ISR was running */
struct Registers {
    uint32_t esp;
    uint32_t edi, esi, ebp, edx, ecx, ebx, eax; 
    uint32_t gs, fs, es, ds;
    uint32_t interuptNumber, errorCode;
    uint32_t eip, cs, eflags, useresp, ss;
};
#endif