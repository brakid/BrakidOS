#include "processmanager.h"
#include "types.h"
#include "ports.h"
#include "regs.h"
#include "io.h"
#include "irqs.h"
#include "utils.h"
#include "memory.h"
#include "constants.h"
#include "process.h"
#include "timer.h"

uint32_t irqStackPointer = (uint32_t)(HEAP_START - 1);
uint32_t stackPointer = 0;

Process* currentProcess = 0;
byte processId = 0;

byte getCurrentProcessId() {
    return currentProcess->processId;
}

byte getNewProcessId() {
    return processId++;
}

void unblockProcesses() {
    Node<Process>* processNode = processes;
    processNode = processNode->next;
    while(processNode != 0) {
        processNode = processNode->next;
    }
}

Process* selectNextProcess(Process* current) {
    Process* first = processes->value;
    Process* second = processes->next->value;
    Process* third = processes->next->next->value;
    if (current == first) {
        return second;
    } else if (current == second) {
        return third;
    } else {
        return first;
    }
    /*Node<Process>* processNode = getProcessNode(current)->next;
    if (processNode == 0) {
        processNode = processes;
    }
    bool found = false;
    while(!found) {
        Process* process = processNode->value;
        if (process->processState == READY) {
            found = true;
            return process;
        }
        processNode = processNode->next;
        if (processNode == 0) {
            processNode = processes;
        }
        if (processNode == getProcessNode(current) && current->processState != READY) {
            println("No runnable processes");
            return 0;
        }
    }
    return 0;*/
}

/* timer: ~18.22Hz */
extern "C" void timerHandler() {
    saveContext();
    // save current stackpointer
    asm("mov stackPointer, esp");
    //switch to internal stackpointer
    asm("mov esp, irqStackPointer");
    // save current process stack checksum
    Process* current = currentProcess;
    current->stackTopPointer = (uint32_t*)stackPointer;
    
    increaseTimer();

    //set current process to READY
    if (current->processState == RUNNING) {
        current->processState = READY;   
    }
    if (current->processState == FINISHED) {
        //free memory
        free((uint32_t*)currentProcess->stackTopPointer);
        free((uint32_t*)currentProcess);
    }

    // unblock blocked processes
    unblockProcesses();
    
    // round robin like scheduling
    currentProcess = selectNextProcess(current);
   
    //currentProcess = processNode->value;
    current = currentProcess; // implement scheduling strategy
    current->processState = RUNNING;
    // check stack checksum
    
    //set stackpointer back
    stackPointer = (uint32_t)current->stackTopPointer;
    println(stackPointer);
    asm("mov esp, stackPointer");
    restoreContext();
    port_byte_out(0x20, 0x20);
    asm("iret");
}

void dispatchProcess() {
    enterCritical();
    ProgramFunction program = currentProcess->programPointer;
    if (program == 0) {
        println("Invalid program");
        leaveCritical();
        return;
    }
    leaveCritical();
    program();
    killProcess();
    yield();
}

void killProcess() {
    enterCritical();
    if (currentProcess->processId != KERNEL_PROCESS_ID) {
        currentProcess->processState = FINISHED;
        Node<Process>* processNode = processes;
        Node<Process>* previous = 0;
        
        while(processNode != 0) {
            if (processNode->value == currentProcess) {
                if (previous == 0) {
                    processes = processNode->next;
                } {
                    previous->next = processNode->next;
                }
                free((uint32_t*)processNode);
                break;
            }
            previous = processNode;
            processNode = processNode->next;
        } 
    }
    leaveCritical();
}

void yield() {
    enterCritical();
    uint32_t critical = criticalCount;
    criticalCount = 1;
    if (currentProcess->processState != FINISHED) {
        currentProcess->processState = BLOCKED;
    }
    leaveCritical();
    timerHandler();
    enterCritical();
    criticalCount = critical;
    leaveCritical();
}

void startProcess(Process* process) {
    enterCritical();
    print("Starting process with id ");
    println(process->processId);
    *process->stackTopPointer-- = EFLAGS_VALUE;
    *process->stackTopPointer-- = CS_VALUE;
    *process->stackTopPointer-- = (uint32_t)&dispatchProcess;
    for (int i = 0; i < 19; i++) { // fill with 12 zeros -> "restoreContext()"
        *process->stackTopPointer-- = 0x0;
    }
    process->stackTopPointer++;
    process->processState = READY;
    
    leaveCritical(); 
}

void startProcessManager() {
    enterCritical();
    if (processes == 0) {
        println("No processes to run");
        return;
    }
    Node<Process>* processNode = processes;
    while(processNode != 0) {
        Process* process = processNode->value;
        startProcess(process);
        processNode = processNode->next;
    }
    currentProcess = processes->value;
    currentProcess->processState = RUNNING;

    // check stack checksum
    //set stackpointer to process stackpointer
    stackPointer = (uint32_t)currentProcess->stackTopPointer;
    asm("mov esp, stackPointer");
    restoreContext();
    leaveCritical();
    asm("iret");
}