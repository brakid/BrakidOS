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
    /*println("Hello world!");
    wait(5000);
    println("Slept 5s");
    print("Last Character: ");
    char c = 0;
    while (c != '\n') {
        if (c != 0) {
            print(c);
        }
        c = getLastCharacter();
    }
    println("\nTerminate");*/
    uint32_t* p1 = malloc(2);
    println((uint32_t)p1);
    uint32_t* p2 = malloc(1);
    println((uint32_t)p2);  //+8
    free(p1);
    uint32_t* p3 = malloc(3); // +12
    println((uint32_t)p3); 
    uint32_t* p4 = malloc(1); // +0
    println((uint32_t)p4); 
}