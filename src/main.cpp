#include "io.h"
#include "global.h"
#include "idt.h"
#include "timer.h"
#include "utils.h"
#include "keyboard.h"

void setup() {
    disableInterupts();
    // GDT is instantiated by bootloader
    installIdt();
    installTimer();
    installKeyboard();

    enableInterupts();
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
    println(getLastCharacter());
}