#ifndef __TIMER__
#define __TIMER__
#include "types.h"

#define CLOCK_FREQUENCY 1193180 // 1.19MHz
#define CLOCK_STEPS_PER_SECOND 18 // exactly: 18.22Hz

void installTimer();
uint64_t getTime();
void wait(uint32_t milliseconds);
#endif