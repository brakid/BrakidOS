#include "programs.h"
#include "io.h"
#include "types.h"
#include "timer.h"
#include "processmanager.h"
#include "keyboard.h"
#include "memory.h"
#include "utils.h"

void program1Function() {
    char* string = (char*)malloc(10);
    println("Enter number 1:");
    int x = atoi(scan(10, string));
    println("Enter number 2:");
    int y = atoi(scan(10, string));
    print("Sum: ");
    println(x + y);
    wait(50);
    println("Finish");
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