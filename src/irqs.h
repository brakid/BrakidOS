#ifndef __IRQS__
#define __IRQS__

void installIrqHandler(int irq, void (*handler)(int));
void uninstallIrqHandler(int irq);
void installIrqs();
#endif