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
    uint32_t* stackBottomPointer;
    ProcessState processState;
    uint32_t checksum;
};

void initProcesses();

void initProcess(Process* process);

void cleanupProcess(Process* process);

List* getProcessList();
Process* getIdleProcess();

bool validateStack(Process* process);
uint32_t calculateChecksum(Process* process);
#endif