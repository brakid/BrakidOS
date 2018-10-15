#ifndef __SCHEDULINGSTRATEGIES__
#define __SCHEDULINGSTRATEGIES__

#include "types.h"
#include "process.h"

typedef Process* (*SchedulingStrategy)(Process*);

extern int selectedSchedulingStrategy;

Process* roundRobin(Process* currentProcess);

SchedulingStrategy getSelectedSchedulingStrategy();

#endif