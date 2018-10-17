#ifndef __PROCESSMANAGER__
#define __PROCESSMANAGER__

#include "process.h"
#include "types.h"

#define CS_VALUE 0x8
#define EFLAGS_VALUE 0x202 //interupts enabled

extern Process* currentProcess;

byte getNewProcessId();

void startProcessManager();
void killProcess(Process* process);
void startProcess(Process* process);
void showProcessManager();

#endif