#include "types.h"
#include "utils.h"
#include "isrs.h"
#include "irqs.h"
#include "idt.h"
#include "io.h"

/* Declare an IDT of 256 entries. Although we will only use the
*  first 32 entries in this tutorial, the rest exists as a bit
*  of a trap. If any undefined IDT entry is hit, it normally
*  will cause an "Unhandled Interrupt" exception. Any descriptor
*  for which the 'presence' bit is cleared (0) will generate an
*  "Unhandled Interrupt" exception */
struct IdtEntry idt[IDT_SIZE];
struct IdtPointer idtp;

/* This exists in 'bootloadercpp.asm', and is used to load our IDT */
extern "C" void idtLoad(uint32_t* p);

// set a single IDT entry
void idtSetGate(uint8_t num, uint32_t base, uint16_t select, uint8_t flags) {
    /* The interrupt routine's base address */
    idt[num].baseLow = (base & 0xFFFF);
    idt[num].baseHigh = 0;//(base >> 16) & 0xFFFF;
    /* The segment or 'selector' that this IDT entry will use
    *  is set here, along with any access flags */
    idt[num].select = select;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

/* Installs the IDT */
void installIdt() {
    /* Sets the special IDT pointer up */
    idtp.limit = (sizeof(IdtEntry) * IDT_SIZE) - 1;
    idtp.base = (uint32_t)&idt;

    byte* byteIdt = (byte*)&idt;

    /* Clear out the entire IDT, initializing it to zeros */
    memset(byteIdt, 0, sizeof(IdtEntry) * IDT_SIZE);
    
    /* Add any new ISRs to the IDT here using idt_set_gate */
    installIsrs();
    installIrqs();

    asm("lidt [idtp]");
}