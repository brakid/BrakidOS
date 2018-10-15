#include "programs.h"
#include "io.h"
#include "types.h"
#include "timer.h"
#include "processmanager.h"

void program1Function() {
    byte counter = 0;
    while (true) {
        print("Program 1: ");
        println(counter++);
        wait(50);
    }
}

void program2Function() {
    byte counter = 0;
    while (true) {
        print("Program 2: ");
        println(counter--);
        wait(50);
    }
}

void program3Function() {
    byte counter = 0;
    while (counter < 10) {
        print("Program 3: ");
        println(counter++);
        wait(500);
    }
}