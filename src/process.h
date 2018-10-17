#ifndef __PROCESSES__
#define __PROCESSES__

#include "types.h"
#include "program.h"
#include "regs.h"
#include "process.h"
#include "list.h"

#define NOT_STARTED_NAME "Not Started"
#define RUNNING_NAME "Running"
#define READY_NAME "Ready"
#define FINISHED_NAME "Finished"
#define BLOCKED_NAME "Waiting"

enum ProcessState {
    NOT_STARTED, RUNNING, READY, FINISHED, BLOCKED
};

struct Process {
    uint32_t processId;
    Program* program;
    uint32_t* stackTopPointer;
    uint32_t* stackBottomPointer;
    ProcessState processState;
    uint32_t checksum;
};

void initProcesses();

Process* initProcess(Program* program);

void cleanupProcess(Process* process);

List* getProcessList();
Process* getIdleProcess();

bool validateStack(Process* process);
uint32_t calculateChecksum(Process* process);
#endif