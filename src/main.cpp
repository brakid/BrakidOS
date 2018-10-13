#include "io.h"
#include "global.h"
#include "idt.h"
#include "timer.h"
#include "utils.h"
#include "keyboard.h"

void setup() {
    enterCritical();
    // GDT is instantiated by bootloader
    installIdt();
    installTimer();
    installKeyboard();

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
    print("Last Character: ");
    char c = 0;
    while (c != '\n') {
        if (c != 0) {
            print(c);
        }
        c = getLastCharacter();
    }
    println("\nTerminate");
}