#include "timer.h"
#include "ports.h"
#include "regs.h"
#include "types.h"
#include "processmanager.h"
#include "io.h"

volatile uint64_t timerTicks = 0;

void increaseTimer() {
    timerTicks++;
}

void setTimerPhase(int frequency){
    int divisor = CLOCK_FREQUENCY / frequency; /* Calculate our divisor */
    port_byte_out(0x43, 0x34);             /* Set our command byte 0x34 */
    port_byte_out(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    port_byte_out(0x40, divisor >> 8);     /* Set high byte of divisor */
}

// returns time in Milliseconds since system start
uint64_t getTime() {
    return timerTicks * (1000 / CLOCK_STEPS_PER_SECOND);
}

void wait(uint32_t milliseconds) {
    uint64_t endMilliseconds = getTime() + milliseconds;
    while(endMilliseconds > getTime());
}