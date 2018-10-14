#include "process.h"
#include "types.h"
#include "memory.h"
#include "io.h"
#include "program.h"
#include "constants.h"
#include "utils.h"
#include "processmanager.h"

Node<Process>* processes = 0;
Node<Process>* lastProcess = 0;

Node<Process>* getProcessNode(Process* process) {
    enterCritical();
    Node<Process>* processNode = processes;
    while (processNode != 0) {
        if (processNode->value == process) {
            return processNode;
        }
        processNode = processNode->next;
    }
    print("No process node found");
    leaveCritical();
    return 0;
}

Node<Process>* initProcess(Program* program) {
    enterCritical();
    Node<Process>* processNode = (Node<Process>*)malloc(sizeof(Node<Process>)/ALLOCATION_SIZE);
    Process* process = (Process*)malloc(sizeof(Process)/ALLOCATION_SIZE);
    processNode->value = process;
    process->processId = getNewProcessId();
    process->processState = NOT_STARTED;
    process->programPointer = program->programFunction;
    process->stackTopPointer = (uint32_t*)(malloc(PROCESS_STACK_SIZE/ALLOCATION_SIZE) + (PROCESS_STACK_SIZE/ALLOCATION_SIZE)) - 1; //16KB stack size, point to end
    if (lastProcess != 0) {
        lastProcess->next = processNode;
    }
    lastProcess = processNode;
    print("Initializing program ");
    print(program->programId);
    print(" as process ");
    print(process->processId);
    print(" with stack ");
    println((uint32_t)process->stackTopPointer);
    leaveCritical();
    return processNode;
}

void initProcesses() {
    Node<Program>* programNode = getPrograms();
    // append all AUTOSTART processes
    while (programNode != 0) {
        Program* program = programNode->value;
        if (program->programType == AUTOSTART) {
            Node<Process>* processNode = initProcess(program);
            if (processes == 0) {
                processes = processNode;
            }
        }
        programNode = programNode->next;
    }
}