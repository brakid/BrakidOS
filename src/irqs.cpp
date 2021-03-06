#include "types.h"
#include "isrs.h"
#include "io.h"
#include "idt.h"
#include "ports.h"
#include "regs.h"
#include "irqs.h"

/* These are function prototypes for all of the exception
*  handlers: The first 32 entries in the IDT are reserved
*  by Intel, and are designed to service exceptions! */
extern "C" void irq0();
extern "C" void irq1();
extern "C" void irq2();
extern "C" void irq3();
extern "C" void irq4();
extern "C" void irq5();
extern "C" void irq6();
extern "C" void irq7();
extern "C" void irq8();
extern "C" void irq9();
extern "C" void irq10();
extern "C" void irq11();
extern "C" void irq12();
extern "C" void irq13();
extern "C" void irq14();
extern "C" void irq15();

// remap: http://www.osdever.net/bkerndev/Docs/irqs.htm
void irqRemap() {
    // mapping IRQ0 -> 32
    port_byte_out(PIC_1_CTRL, 0x11);
    port_byte_out(PIC_2_CTRL, 0x11);
    port_byte_out(PIC_1_DATA, IRQ_OFFSET);
    port_byte_out(PIC_2_DATA, IRQ_OFFSET + 8);
    port_byte_out(PIC_1_DATA, 0x04);
    port_byte_out(PIC_2_DATA, 0x02);
    port_byte_out(PIC_1_DATA, 0x01);
    port_byte_out(PIC_2_DATA, 0x01);

    // enable (masking all IRQS as 0 = enabled)
    port_byte_out(PIC_1_DATA, 0x0);
    port_byte_out(PIC_2_DATA, 0x0);
}

// pointers to routines for each IRW
uint32_t irqRoutines[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

void installIrqHandler(int irq, IrqHandler handler) {
    irqRoutines[irq] = (uint32_t)handler;
}

void uninstallIrqHandler(int irq) {
    irqRoutines[irq] = 0;
}

void installIrqs() {
    irqRemap();

    idtSetGate(32, (uint32_t)irq0, 0x08, 0x8E);
    idtSetGate(33, (uint32_t)irq1, 0x08, 0x8E);
    idtSetGate(34, (uint32_t)irq2, 0x08, 0x8E);
    idtSetGate(35, (uint32_t)irq3, 0x08, 0x8E);
    idtSetGate(36, (uint32_t)irq4, 0x08, 0x8E);
    idtSetGate(37, (uint32_t)irq5, 0x08, 0x8E);
    idtSetGate(38, (uint32_t)irq6, 0x08, 0x8E);
    idtSetGate(39, (uint32_t)irq7, 0x08, 0x8E);
    idtSetGate(40, (uint32_t)irq8, 0x08, 0x8E);
    idtSetGate(41, (uint32_t)irq9, 0x08, 0x8E);
    idtSetGate(42, (uint32_t)irq10, 0x08, 0x8E);
    idtSetGate(43, (uint32_t)irq11, 0x08, 0x8E);
    idtSetGate(44, (uint32_t)irq12, 0x08, 0x8E);
    idtSetGate(45, (uint32_t)irq13, 0x08, 0x8E);
    idtSetGate(46, (uint32_t)irq14, 0x08, 0x8E);
    idtSetGate(47, (uint32_t)irq15, 0x08, 0x8E);
}

extern "C" void irqHandler(Registers* registers) {
    uint32_t irqNumber = registers->interuptNumber;
    
    IrqHandler handler = (IrqHandler)irqRoutines[irqNumber - IRQ_OFFSET];
    if (handler != 0) {
        handler((Registers*)registers);
    }

    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 7), then we need to send an EOI to
    *  the slave controller */
    if (irqNumber >= 40) {
        port_byte_out(PIC_2_CTRL, 0x20);
    }

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    port_byte_out(PIC_1_CTRL, 0x20);
}

void setIrqs(uint16_t mask) {
    byte lowerMask = (byte)(mask & 0x0000FFFF);
    byte higherMask = (byte)((mask >> 8) & 0x0000FFFF);
    
    port_byte_out(PIC_1_DATA, lowerMask);
    port_byte_out(PIC_2_DATA, higherMask);
}