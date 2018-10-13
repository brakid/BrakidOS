#ifndef __IRQS__
#define __IRQS__
#include "regs.h"

#define IRQ_OFFSET 32

void installIrqHandler(int irq, void (*handler)(Registers*));
void uninstallIrqHandler(int irq);
void installIrqs();
#endif