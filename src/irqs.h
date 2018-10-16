#ifndef __IRQS__
#define __IRQS__
#include "regs.h"
#include "types.h"

#define IRQ_OFFSET 32

typedef void (*IrqHandler)(Registers*);

void installIrqHandler(int irq, IrqHandler handler);
void uninstallIrqHandler(int irq);
void installIrqs();

void setIrqs(uint16_t mask);
#endif