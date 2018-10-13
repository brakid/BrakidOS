#include "types.h"
#include "isrs.h"
#include "io.h"
#include "idt.h"
#include "regs.h"

/* These are function prototypes for all of the exception
*  handlers: The first 32 entries in the IDT are reserved
*  by Intel, and are designed to service exceptions! */
extern "C" void isr0();
extern "C" void isr1();
extern "C" void isr2();
extern "C" void isr3();
extern "C" void isr4();
extern "C" void isr5();
extern "C" void isr6();
extern "C" void isr7();
extern "C" void isr8();
extern "C" void isr9();
extern "C" void isr10();
extern "C" void isr11();
extern "C" void isr12();
extern "C" void isr13();
extern "C" void isr14();
extern "C" void isr15();
extern "C" void isr16();
extern "C" void isr17();
extern "C" void isr18();
extern "C" void isr19();
extern "C" void isr20();
extern "C" void isr21();
extern "C" void isr22();
extern "C" void isr23();
extern "C" void isr24();
extern "C" void isr25();
extern "C" void isr26();
extern "C" void isr26();
extern "C" void isr27();
extern "C" void isr28();
extern "C" void isr29();
extern "C" void isr30();
extern "C" void isr31();

/* We set the access
*  flags to 0x8E. This means that the entry is present, is
*  running in ring 0 (kernel level), and has the lower 5 bits
*  set to the required '14', which is represented by 'E' in
*  hex. */
void installIsrs() {
    idtSetGate(0, (uint32_t)isr0, 0x08, 0x8E);
    idtSetGate(1, (uint32_t)isr1, 0x08, 0x8E);
    idtSetGate(2, (uint32_t)isr2, 0x08, 0x8E);
    idtSetGate(3, (uint32_t)isr3, 0x08, 0x8E);
    idtSetGate(4, (uint32_t)isr4, 0x08, 0x8E);
    idtSetGate(5, (uint32_t)isr5, 0x08, 0x8E);
    idtSetGate(6, (uint32_t)isr6, 0x08, 0x8E);
    idtSetGate(7, (uint32_t)isr7, 0x08, 0x8E);
    idtSetGate(8, (uint32_t)isr8, 0x08, 0x8E);
    idtSetGate(9, (uint32_t)isr9, 0x08, 0x8E);
    idtSetGate(10, (uint32_t)isr10, 0x08, 0x8E);
    idtSetGate(11, (uint32_t)isr11, 0x08, 0x8E);
    idtSetGate(12, (uint32_t)isr12, 0x08, 0x8E);
    idtSetGate(13, (uint32_t)isr13, 0x08, 0x8E);
    idtSetGate(14, (uint32_t)isr14, 0x08, 0x8E);
    idtSetGate(15, (uint32_t)isr15, 0x08, 0x8E);
    idtSetGate(16, (uint32_t)isr16, 0x08, 0x8E);
    idtSetGate(17, (uint32_t)isr17, 0x08, 0x8E);
    idtSetGate(18, (uint32_t)isr18, 0x08, 0x8E);
    idtSetGate(19, (uint32_t)isr19, 0x08, 0x8E);
    idtSetGate(20, (uint32_t)isr20, 0x08, 0x8E);
    idtSetGate(21, (uint32_t)isr21, 0x08, 0x8E);
    idtSetGate(22, (uint32_t)isr22, 0x08, 0x8E);
    idtSetGate(23, (uint32_t)isr23, 0x08, 0x8E);
    idtSetGate(24, (uint32_t)isr24, 0x08, 0x8E);
    idtSetGate(25, (uint32_t)isr25, 0x08, 0x8E);
    idtSetGate(26, (uint32_t)isr26, 0x08, 0x8E);
    idtSetGate(27, (uint32_t)isr27, 0x08, 0x8E);
    idtSetGate(28, (uint32_t)isr28, 0x08, 0x8E);
    idtSetGate(29, (uint32_t)isr29, 0x08, 0x8E);
    idtSetGate(30, (uint32_t)isr30, 0x08, 0x8E);
    idtSetGate(31, (uint32_t)isr31, 0x08, 0x8E);
}

/* All of our Exception handling Interrupt Service Routines will
*  point to this function. This will tell us what exception has
*  happened! Right now, we simply halt the system by hitting an
*  endless loop. All ISRs disable interrupts while they are being
*  serviced as a 'locking' mechanism to prevent an IRQ from
*  happening and messing up kernel data structures */
extern "C" void faultHandler(Registers* registers) {
    uint32_t interuptNumber = registers->interuptNumber;

    println("");
    print("Exception: ");
    println(interuptNumber);
    println("System Halted!");
    for(;;);
}