#ifndef __PROCESSES__
#define __PROCESSES__

#include "types.h"
#include "program.h"
#include "regs.h"
#include "process.h"
#include "list.h"

enum ProcessState {
    NOT_STARTED, RUNNING, READY, FINISHED, BLOCKED
};

struct Process {
    uint32_t processId;
    ProgramFunction programPointer;
    uint32_t* stackTopPointer;
    ProcessState processState;
};

extern Node<Process>* processes;
extern Node<Process>* lastProcess;

Node<Process>* getProcessNode(Process* process);

void initProcesses();

void initProcess(Process* process);
#endif