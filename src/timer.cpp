#include "timer.h"
#include "ports.h"
#include "regs.h"
#include "io.h"
#include "irqs.h"

#define CLOCK_FREQUENCY 1193180 // 1.19MHz
#define CLOCK_STEPS_PER_SECOND 18 // exactly: 18.22Hz


volatile uint64_t timerTicks = 0;

void setTimerPhase(int frequency){
    int divisor = CLOCK_FREQUENCY / frequency;       /* Calculate our divisor */
    port_byte_out(0x43, 0x36);             /* Set our command byte 0x36 */
    port_byte_out(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    port_byte_out(0x40, divisor >> 8);     /* Set high byte of divisor */
}

/* timer: ~18.22Hz */
void handleTimer(int n){//Registers* registers){
    n++;
    timerTicks++;

    if (timerTicks % CLOCK_STEPS_PER_SECOND == 0) {
        println("One second has passed\n");
    }
}

void installTimer(){
    setTimerPhase(100);
    // IRQ0: timer
    installIrqHandler(0, handleTimer);
}

uint64_t getTime() {
    return timerTicks * 1.0 / CLOCK_STEPS_PER_SECOND;
}