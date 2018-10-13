#ifndef __REGS__
#define __REGS__
#include "types.h"

/* This defines what the stack looks like after an ISR was running */
struct Registers {
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int interuptNumber, errorCode;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};
#endif