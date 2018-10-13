#include "keyboard.h"
#include "ports.h"
#include "regs.h"
#include "io.h"
#include "irqs.h"
#include "types.h"

char lastCharacter = 0;

/* timer: ~18.22Hz */
void handleKeyboard(Registers* registers){
    // store in allocated memory
    byte keyboardUs[128] = 
    {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
        '9', '0', '-', '=', '\b', /* Backspace */
        '\t',     /* Tab */
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
        0,      /* 29   - Control */
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,    /* Left shift */
        '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',  0,        /* Right shift */
        '*',
        0,  /* Alt */
        ' ',  /* Space bar */
        0,  /* Caps lock */
        0,  /* 59 - F1 key ... > */
        0,   0,   0,   0,   0,   0,   0,   0,
        0,  /* < ... F10 */
        0,  /* 69 - Num lock*/
        0,  /* Scroll Lock */
        0,  /* Home key */
        0,  /* Up Arrow */
        0,  /* Page Up */
        '-',
        0,  /* Left Arrow */
        0,
        0,  /* Right Arrow */
        '+',
        0,  /* 79 - End key*/
        0,  /* Down Arrow */
        0,  /* Page Down */
        0,  /* Insert Key */
        0,  /* Delete Key */
        0,   0,   0,
        0,  /* F11 Key */
        0,  /* F12 Key */
        0,  /* All other keys are undefined */
    };

    uint32_t interupt = registers->interuptNumber;
    interupt++;
    uint8_t keycode = port_byte_in(0x60);
    /* Lowest bit of status will be set if buffer is not empty */
    if (keycode & 0x80) {
        //control keys
    } else {
        if (keycode < 128) {
            lastCharacter = keyboardUs[keycode];
        }
    }
}

char getLastCharacter() {
    byte last = lastCharacter;
    lastCharacter = 0;
    return last;
}

void installKeyboard(){
    // IRQ1: keyboard
    installIrqHandler(1, handleKeyboard);
}