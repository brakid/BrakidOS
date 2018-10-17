#include "processmanager.h"
#include "types.h"
#include "ports.h"
#include "regs.h"
#include "io.h"
#include "utils.h"
#include "memory.h"
#include "constants.h"
#include "process.h"
#include "program.h"
#include "timer.h"
#include "schedulingstrategy.h"
#include "list.h"
#include "scheduler.h"
#include "keyboard.h"
#include "irqs.h"

List* programs = 0;
byte processId = 0;
uint32_t stack = 0;

byte getNewProcessId() {
    return processId++;
}

void dispatchProcess() {
    enterCritical();
    ProgramFunction program = currentProcess->program->programFunction;
    if (program == 0) {
        println("Invalid program");
        leaveCritical();
        return;
    }
    leaveCritical();
    program();
    killProcess(currentProcess);
    yield();
}

void killProcess(Process* process) {
    enterCritical();
    if (process->processId != KERNEL_PROCESS_ID) {
        println("Finished");
        process->processState = FINISHED;
    }
    leaveCritical();
}

void startProcess(Process* process) {
    enterCritical();
    print("Starting process with id ");
    println(process->processId);
    *process->stackTopPointer-- = EFLAGS_VALUE;
    *process->stackTopPointer-- = CS_VALUE;
    *process->stackTopPointer-- = (uint32_t)&dispatchProcess;
    for (int i = 0; i < 19; i++) { // fill with 19 zeros -> "restoreContext()"
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

    List * processes = getProcessList();
    if (size(processes) == 0) {
        println("No processes to run");
        currentProcess = getIdleProcess();
    } else {    
       /* for (int index = 0; index < size(processes); index++) {
            Process* process = (Process*)get(processes, index);
            startProcess(process);
        }
        currentProcess = (Process*)get(processes, 0);*/
    }
    currentProcess = getIdleProcess();
    currentProcess->processState = RUNNING;

    // check stack checksum
    //set stackpointer to process stackpointer
    stack = (uint32_t)currentProcess->stackTopPointer;
    asm("mov esp, stack");
    restoreContext();
    leaveCritical();
    asm("iret");
}

void printProcessStatus(Process* process) {
    switch(process->processState) {
        case READY: 
            print(READY_NAME);
            break;
        case RUNNING: 
            print(RUNNING_NAME);
            break;
        case BLOCKED: 
            print(BLOCKED_NAME);
            break;
        case FINISHED: 
            print(FINISHED_NAME);
            break;
        case NOT_STARTED: 
            print(NOT_STARTED_NAME);
            break;
    }
}

void printProcessState(Process* process) {
    if (getCurrentProcessId() == process->processId) {
        print("*");
    } else {
        print(" ");
    }
    print("Process ");
    print(process->processId);
    print(" - Program ");
    print(process->program->programId);
    print(" - Status: ");
    printProcessStatus(process);
    println("");
}

void printProgramType(Program* program) {
    switch(program->programType) {
        case AUTOSTART: 
            print(AUTOSTART_NAME);
            break;
        case MANUAL: 
            print(MANUAL_NAME);
            break;
    }
}

void printProgramState(Program* program) {
    print("Program ");
    print(program->programId);
    print(" - Type: ");
    printProgramType(program);
    println("");
}

void printOverview() {
    println("Running processes:");
    List* processes = getProcessList();
    for (int index = 0; index < size(processes); index++) {
        printProcessState((Process*)get(processes, index));   
    }
    println("\nAvailable programs:");
    List* programs = getProgramList();
    for (int index = 0; index < size(programs); index++) {
        printProgramState((Program*)get(programs, index));
    }
}

Process* getProcess(List* processes, int id) {
    for (int index = 0; index < size(processes); index++) {
        Process* process = (Process*)get(processes, index);
        if (process->processId == (uint32_t)id) {
            return process;
        }   
    }
    return 0;
}

void showStartProcess() {
    List* processes = getProcessList();
    println("Select process to start:");
    char* string = (char*)malloc(100);
    print("Process id: ");
    Process* process = getProcess(processes, atoi(interuptFreeScan(100, string)));
    while(process == 0 && process->processState == NOT_STARTED) {
        println("Invalid process id. Only processes that are not started can be started.");
        print("Process id: ");
        process = getProcess(processes, atoi(interuptFreeScan(100, string)));
    }
    startProcess(process);
    print("Process ");
    print(process->processId);
    println(" started.");
    free(string);
}

void showKillProcess() {
    List* processes = getProcessList();
    println("Select process to kill:");
    char* string = (char*)malloc(100);
    print("Process id: ");
    Process* process = getProcess(processes, atoi(interuptFreeScan(100, string)));
    while(process == 0 && (process->processState == RUNNING && process->processState != READY && process->processState == BLOCKED)) {
        println("Invalid process id. Only processes that are running, ready or blocked can be killed.");
        print("Process id: ");
        process = getProcess(processes, atoi(interuptFreeScan(100, string)));
    }
    killProcess(process);
    print("Process ");
    print(process->processId);
    println(" killed.");
    free(string);
}

Program* getProgram(List* programs, int id) {
    for (int index = 0; index < size(programs); index++) {
        Program* program = (Program*)get(programs, index);
        if (program->programId == (uint32_t)id) {
            return program;
        }   
    }
    return 0;
}

void showStartProgram() {
    List* programs = getProgramList();
    println("Select program to start:");
    char* string = (char*)malloc(100);
    print("Program id: ");
    Program* program = getProgram(programs, atoi(interuptFreeScan(100, string)));
    while(program == 0) {
        println("Invalid program id.");
        print("Program id: ");
        program = getProgram(programs, atoi(interuptFreeScan(100, string)));
    }
    Process* process = initProcess(program);
    append(getProcessList(), process);
    startProcess(process);
    print("Program ");
    print(program->programId);
    print(" started as process ");
    println(process->processId);
    free(string);
}

bool isValidAction(int action) {
    return action >= 1 && action <= 4;
}

void showProcessManager() {
    port_byte_out(0x20, 0x20);

    setCurrentProcessId(KERNEL_PROCESS_ID);
    
    bool isExit = false;
    char* string = (char*)malloc(100);
    
    //clearScreen();
    //println("          =========================");
    //println("          *    Process Manager    *");
    println("          ========================="); 
    while(!isExit) {
        printOverview();
        println("\nAvailable actions:");
        println("1: Start Process");
        println("2: Start Program");
        println("3: Kill Process");
        println("4: Exit");
        print("Action: ");
        int action = atoi(interuptFreeScan(100, string));
        while(!isValidAction(action)) {
            println("Invalid selected action");
            print("Action: ");
            action = atoi(interuptFreeScan(100, string));
        }
        switch(action) {
            case 1:
                showStartProcess();
                break;
            case 2:
                showStartProgram();
                break;
            case 3: 
                showKillProcess(); 
                break;
            case 4: 
                isExit = true;
                break;
        }
    }

    free(string);
}