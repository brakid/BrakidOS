#include "io.h"
#include "idt.h"
#include "timer.h"
#include "utils.h"
#include "keyboard.h"
#include "memory.h"
#include "process.h"
#include "program.h"
#include "processmanager.h"

void setup() {
    initMemory();

    // GDT is set up as part of the bootloader;
    installIdt();
    installKeyboard();

    setTimerPhase(10);
}

extern "C" void main() {
    enterCritical();
    setup();

    clearScreen();
    println("          =========================");
    println("          * BrakidOS - Experiment *");
    println("          ========================="); 
    initPrograms();
    initProcesses();
    leaveCritical();
    startProcessManager();

    while(1);
}