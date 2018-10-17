#include "process.h"
#include "types.h"
#include "memory.h"
#include "io.h"
#include "program.h"
#include "constants.h"
#include "utils.h"
#include "processmanager.h"

List* processList = 0;
Process* idleProcess = 0;

Process* initProcess(Program* program) {
    enterCritical();
    byte processId = getNewProcessId();
    Process* process = (Process*)pmalloc(sizeof(Process), processId);
    process->processId = processId;
    process->processState = NOT_STARTED;
    process->program = program;
    process->stackBottomPointer = (uint32_t*)((byte*)pmalloc(PROCESS_STACK_SIZE, processId) + PROCESS_STACK_SIZE) - 1; //16KB stack size, point to end
    process->stackTopPointer = process->stackBottomPointer;
    process->checksum = 0;
    print("Initializing program ");
    print(program->programId);
    print(" as process ");
    println(process->processId);
    leaveCritical();
    return process;
}

void initProcesses() {
    enterCritical();
    idleProcess = initProcess(getIdleProgram());

    processList = createList();

    List* programs = getProgramList();
    // append all AUTOSTART processes
    for (int index = 0; index < size(programs); index++) {
        Program* program = (Program*)get(programs, index);
        if (program->programType == AUTOSTART) {
            Process* process = initProcess(program);
            append(processList, process);
        }
    }
    leaveCritical();
}

void cleanupProcess(Process* process) {
    enterCritical();
    print("Process ");
    print(process->processId);
    println(" finished");

    bool removed = remove(processList, process);
    print("Removed process ");
    print(process->processId);
    print("? ");
    println(removed ? "yes" : "no");
    //free memory
    freeProcessMemory(process->processId);
    leaveCritical();
}

List* getProcessList() {
    return processList;
}

Process* getIdleProcess() {
    return idleProcess;
}

bool validateStack(Process* process) {
    return process->checksum == calculateChecksum(process);
}

uint32_t calculateChecksum(Process* process) {
    enterCritical();
    uint32_t checksum = 0;
    uint32_t* stackPointer = process->stackBottomPointer;
    while(stackPointer >= process->stackTopPointer) {
        checksum ^= *stackPointer--;
    }
    leaveCritical();
    return checksum;
}