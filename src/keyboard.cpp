#include "keyboard.h"
#include "ports.h"
#include "regs.h"
#include "io.h"
#include "irqs.h"
#include "types.h"
#include "utils.h"
#include "memory.h"
#include "timer.h"

#define KEYCODE_COUNT 58

char lastCharacter = 0;

char* keycodes = 0;
char* shiftKeycodes = 0;
volatile bool isShift = false;

void fillKeymap() {
    char keycodesUs[KEYCODE_COUNT] = { 0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
        0, 0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's', 'd',
        'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n',
        'm', ',', '.',
        '/', 0, 0, 0, ' '
    };
    
    char shiftKeycodesUs[KEYCODE_COUNT] = { 0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
        0, 0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, 'A', 'S', 'D', 'F',
        'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>',
        '?', 0, 0, 0, ' '
    };

    keycodes = (char*)malloc(KEYCODE_COUNT);
    memcpy((byte*)keycodes, (byte*)keycodesUs, KEYCODE_COUNT);
    shiftKeycodes = (char*)malloc(KEYCODE_COUNT);
    memcpy((byte*)shiftKeycodes, (byte*)shiftKeycodesUs, KEYCODE_COUNT);
}

void handleKeyboard(Registers* registers){
    uint32_t interupt = registers->interuptNumber;
    interupt++;


    byte status = port_byte_in(STATUS_REGISTER); 

    if (status & 0x01) { // lowest status byte = is data available - https://wiki.osdev.org/%228042%22_PS/2_Controller#Status_Register
        byte keycode = port_byte_in(DATA_REGISTER);
        switch(keycode) {
            case 42: // shift pressed
            case 54:
                isShift = true;
                break;
            case 0xaa: // shift released
            case 0xb6:
                isShift = false;
                break;
            default:
                if (keycode < KEYCODE_COUNT) {
                    lastCharacter = isShift ? shiftKeycodes[keycode] : keycodes[keycode];
                }
        }
    } 

    //end of interupt
    port_byte_out(0x20, 0x20);
}

char getLastCharacter() {
    char last = lastCharacter;
    lastCharacter = 0;
    return last;
}

void installKeyboard(){
    // IRQ1: keyboard
    installIrqHandler(1, handleKeyboard);

    // initialize keymap
    fillKeymap();
}

char* scan(int maxLength, char* pointer) {
    setIrqs(0xFFFD);
    
    memset((byte*)pointer, 0, maxLength);
    int length = 0;
    char* currentPosition = pointer;
    char lastCharacter = 0;
    while (lastCharacter != '\n' && length < maxLength) {
        if (lastCharacter != 0) {
            print(lastCharacter);
            *currentPosition++ = lastCharacter;
            length++;
        }
        lastCharacter = getLastCharacter();
    }
    *currentPosition = 0;
    println("");
    setIrqs(0x0);
    return pointer;
}