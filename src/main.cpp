#include "io.h"
#include "global.h"
#include "idt.h"
#include "timer.h"
#include "utils.h"

void setup() {
    disableInterupts();
    // GDT is instantiated by bootloader
    installIdt();
    installTimer();

    enableInterupts();
}

extern "C" void main() {
    setup();

    clearScreen();
    println("          =========================");
    println("          * BrakidOS - Experiment *");
    println("          =========================");
    print("Hello world!");
}