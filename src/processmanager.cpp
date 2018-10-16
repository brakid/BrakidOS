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

uint32_t irqStackPointer = (uint32_t)(HEAP_START - 1);
uint32_t stackPointer = 0;

List* processes = 0;
Process* currentProcess = 0;
byte currentProcessId = KERNEL_PROCESS_ID;
byte processId = 0;

byte getCurrentProcessId() {
    enterCritical();
    if (currentProcess == 0) {
        leaveCritical();
        return KERNEL_PROCESS_ID;
    }
    leaveCritical();
    return currentProcess->processId;
}

byte getNewProcessId() {
    return processId++;
}

void unblockProcesses() {
    enterCritical();
    for (int index = 0; index < size(processes); index++) {
        Process* process = (Process*)get(processes, index);
        if (process->processState == BLOCKED) {
            process->processState = READY;
        }
    }
    leaveCritical();
}

/* timer: ~18.22Hz */
extern "C" void timerHandler() {
    saveContext();
    // save current stackpointer
    asm("mov stackPointer, esp");
    //switch to internal stackpointer
    asm("mov esp, irqStackPointer");
    // save current process stack checksum
    enterCritical();
    Process* current = currentProcess;
    current->stackTopPointer = (uint32_t*)stackPointer;
    current->checksum = calculateChecksum(current);
    
    increaseTimer();

    //set current process to READY
    if (current->processState == RUNNING) {
        current->processState = READY;   
    }
    if (current->processState == FINISHED) {
        cleanupProcess(currentProcess);
    }

    // unblock blocked processes
    unblockProcesses();
    // round robin like scheduling
    SchedulingStrategy schedulingStrategy = getSelectedSchedulingStrategy();
    currentProcess = schedulingStrategy(current);
    if (currentProcess == 0) {
        currentProcess = getIdleProcess();
    }
    
    //currentProcess = processNode->value;
    current = currentProcess; // implement scheduling strategy
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
    leaveCritical();
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
    process->checksum = calculateChecksum(process);
    leaveCritical(); 
}

void startProcessManager() {
    enterCritical();

    // start idle process
    startProcess(getIdleProcess());

    processes = getProcessList();
    if (size(processes) == 0) {
        println("No processes to run");
        currentProcess = getIdleProcess();
    } else {    
        for (int index = 0; index < size(processes); index++) {
            Process* process = (Process*)get(processes, index);
            startProcess(process);
        }
        currentProcess = (Process*)get(processes, 0);
    }
    
    currentProcess->processState = RUNNING;

    // check stack checksum
    //set stackpointer to process stackpointer
    stackPointer = (uint32_t)currentProcess->stackTopPointer;
    asm("mov esp, stackPointer");
    restoreContext();
    leaveCritical();
    asm("iret");
}