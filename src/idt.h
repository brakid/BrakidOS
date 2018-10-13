#ifndef __IDT__
#define __IDT__
#include "types.h"

#define IDT_SIZE 256

/* Defines an IDT entry */
struct IdtEntry {
    uint16_t baseLow;
    uint16_t select;        /* Our kernel segment goes here! */
    uint8_t always0;     /* This will ALWAYS be set to 0! */
    uint8_t flags;       /* Set using the above table! */
    uint16_t baseHigh;
} __attribute__((packed));

struct IdtPointer {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void idtSetGate(uint8_t num, uint32_t base, uint16_t select, uint8_t flags);
void installIdt();
#endif