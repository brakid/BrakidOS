#include "timer.h"
#include "ports.h"
#include "regs.h"
#include "io.h"
#include "irqs.h"

volatile uint64_t timerTicks = 0;

void setTimerPhase(int frequency){
    int divisor = CLOCK_FREQUENCY / frequency; /* Calculate our divisor */
    port_byte_out(0x43, 0x34);             /* Set our command byte 0x34 */
    port_byte_out(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    port_byte_out(0x40, divisor >> 8);     /* Set high byte of divisor */
}

/* timer: ~18.22Hz */
void handleTimer(Registers* registers){
    uint32_t interupt = registers->interuptNumber;
    interupt++;
    
    timerTicks++;

    if (timerTicks % CLOCK_STEPS_PER_SECOND == 0) {
        //println(interupt);
    }
}

void installTimer(){
    // IRQ0: timer
    installIrqHandler(0, handleTimer);
}

// returns time in Milliseconds since system start
uint64_t getTime() {
    return timerTicks * (1000 / CLOCK_STEPS_PER_SECOND);
}

void wait(uint32_t milliseconds) {
    uint64_t endMilliseconds = getTime() + milliseconds;
    while(endMilliseconds > getTime()){}
}