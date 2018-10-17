#include "scheduler.h"
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
#include "schedulingstrategy.h"
#include "list.h"
#include "keyboard.h"

uint32_t irqStackPointer = (uint32_t)(HEAP_START - 1);
uint32_t stackPointer = 0;

Process* currentProcess = 0;
byte currentProcessId = KERNEL_PROCESS_ID;

void setCurrentProcessId(byte processId) {
    enterCritical();
    currentProcessId = processId;
    leaveCritical();
}

void setCurrentProcess(Process* process) {
    enterCritical();
    currentProcess = process;
    currentProcessId = process->processId;
    leaveCritical();
}

byte getCurrentProcessId() {
    return currentProcessId;
}

void unblockProcesses() {
    enterCritical();
    List* processes = getProcessList();
    for (int index = 0; index < size(processes); index++) {
        Process* process = (Process*)get(processes, index);
        if (process->processState == BLOCKED) {
            process->processState = READY;
        }
    }
    leaveCritical();
}

void cleanupProcesses() {
    enterCritical();
    List* processes = getProcessList();
    for (int index = 0; index < size(processes); index++) {
        Process* process = (Process*)get(processes, index);
        if (process->processState == FINISHED) {
            cleanupProcess(process);
        }
    }
    leaveCritical();
}

void invokeScheduler() {
    // cause timer interupt to call scheduler
    asm("int 32");
}

/* timer: ~18.22Hz */
extern "C" void timerHandler() {
    saveContext();
    // save current stackpointer
    asm("mov stackPointer, esp");
    //switch to internal stackpointer
    asm("mov esp, irqStackPointer");
    // save current process stack checksum
    setIrqs(0xFFFD);
    Process* current = currentProcess;
    current->stackTopPointer = (uint32_t*)stackPointer;
    current->checksum = calculateChecksum(current);
    
    // kernel mode
    setCurrentProcessId(KERNEL_PROCESS_ID);
    
    increaseTimer();

    // check if ESC was pressed
    if (getLastCharacter() == ESC) {
        showProcessManager();
    }

    //set current process to READY
    if (current->processState == RUNNING) {
        current->processState = READY;   
    }
    
    // cleanup finished processes
    cleanupProcesses();
    // unblock blocked processes
    unblockProcesses();
    // round robin like scheduling
    SchedulingStrategy schedulingStrategy = getSelectedSchedulingStrategy();
    current = schedulingStrategy(current);
    if (current == 0) {
        current = getIdleProcess();
    }
    
    // set new current process id
    setCurrentProcess(current);
    current->processState = RUNNING;
    
    // check stack checksum
    if (!validateStack(current)) {
        println("Stack content has changed!");
        for(;;);
    }
    //set stackpointer back
    stackPointer = (uint32_t)current->stackTopPointer;
    asm("mov esp, stackPointer");
    restoreContext();
    setIrqs(0x0);
    port_byte_out(0x20, 0x20);
    asm("iret");
}

void yield() {
    enterCritical();
    uint32_t critical = criticalCount;
    criticalCount = 1;
    if (currentProcess->processState != FINISHED) {
        currentProcess->processState = BLOCKED;
    }
    leaveCritical();
    invokeScheduler();
    enterCritical();
    criticalCount = critical;
    leaveCritical();
}