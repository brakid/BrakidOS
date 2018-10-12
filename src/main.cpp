#include "io.h"
#include "global.h"

extern "C" void main() {
    Io::clearScreen();
    Io::println("          =========================");
    Io::println("          * BrakidOS - Experiment *");
    Io::println("          =========================");
    Io::print("Hello world!");
    Io::print('X');
    byte i = 42;
    byte j = 41;
    Io::println(i);
    Io::println((uint32_t)&i);
    Io::println((uint32_t)&j);
}