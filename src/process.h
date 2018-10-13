#ifndef __PROCESSES__
#define __PROCESSES__

enum ProcessState {
    NOT_STARTED, RUNNING, FINISHED
}

struct Process {
    byte processId;
    uint32_t* programmPointer;
    uint32_t* stackTopPointer;
    ProcessState processState;
};
#endif