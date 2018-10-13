#include "io.h"
#include "global.h"
#include "idt.h"
#include "timer.h"
#include "utils.h"
#include "keyboard.h"
#include "memory.h"

void setup() {
    enterCritical();
    // GDT is set up as part of the bootloader;
    installIdt();
    installTimer();
    installKeyboard();

    initMemory();

    leaveCritical();
}

extern "C" void main() {
    setup();

    clearScreen();
    println("          =========================");
    println("          * BrakidOS - Experiment *");
    println("          =========================");
    println("Hello world!");
    wait(5000);
    println("Slept 5s");
    char* userInput = (char*)malloc(100 / sizeof(uint32_t));
    print(">: ");
    userInput = scan(100, userInput);
    println("\nTerminate");
    println(userInput);
    free((uint32_t*)userInput);
}