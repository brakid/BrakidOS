#include "io.h"
#include "global.h"

extern "C" void main() {
    Io::clearScreen();
    Io::println("          =========================");
    Io::println("          * BrakidOS - Experiment *");
    Io::println("          =========================");
    Io::print("Hello world!");
}