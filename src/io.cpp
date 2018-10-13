#include "global.h"
#include "io.h"
#include "string.h"
#include "ports.h"
#include "utils.h"

byte row = 0;
byte column = 0;

void updateCursor(byte row, byte column){
    int position = (row * COLUMNS + column);
 
    port_byte_out(VGA_INDEX_REGISTER, 0x0F);
    port_byte_out(VGA_DATA_REGISTER, (uint8_t) (position & 0xFF));
    port_byte_out(VGA_INDEX_REGISTER, 0x0E);
    port_byte_out(VGA_DATA_REGISTER, (uint8_t) ((position >> 8) & 0xFF));
}

// because of printing a single character each time the maximum row can be 25;
void scrollScreen() {
    uint16_t blank = 0x20 | WHITE << 8;
    volatile char* video = (volatile char*) VIDEO_POSITION;

    if (row >= ROWS) {
        memcpy((uint16_t*)video, (uint16_t*)video + COLUMNS, (ROWS - 1) * COLUMNS);
        memset((uint16_t*)video + (ROWS - 1) * COLUMNS, blank, COLUMNS);
        row -= 1;
    }
}

void newLine() {
    row++;
    column = 0;
    scrollScreen();
    updateCursor(row, column);
}

void print(
        const char* string, 
        byte color) {
    while(*string != 0) {
        char character = *string++;
        if (character == '\n') {
            newLine();
        }
        print(character, color);
    }
}

void print( 
        char character, 
        byte color) {
    disableInterupts();
    volatile char* video = (volatile char*) VIDEO_POSITION;
    video += (row * COLUMNS + column) * sizeof(char) * 2;
    
    *video++ = character;
    *video++ = color;
    column++;
    
    if (column > COLUMNS) {
        byte rows = column / COLUMNS;
        column -= rows * COLUMNS;
        row += rows; 
    }
    scrollScreen();
    updateCursor(row, column);
    enableInterupts();
}

void print(
        byte number,
        byte color) {
    print((uint32_t)number, color);
}

void print(
        int number,
        byte color) {
    if (number < 0) {
        print('-', color);
        number *= -1;
    }
    print((uint32_t)number, color);
}

void print(
        uint32_t number,
        byte color) {
    uint32_t compare = 0; //1000000000, HEX: 0x3b9aca00
    uint32_t address = (uint32_t)&compare;
    byte* bytes = (byte*)address;
    //LSB first
    *bytes++ = 0x00;
    *bytes++ = 0xca;    
    *bytes++ = 0x9a;
    *bytes++ = 0x3b;


    bool hasPrinted = false;
    while (compare > 0) {
        byte digit = number / compare;
        number -= digit * compare;
        compare /= 10;
        if (digit != 0 || hasPrinted == true || compare == 0) {
            print(digitToChar(digit), color);
            hasPrinted = true;
        }
    }
}

void println( 
        const char* string, 
        byte color) {
    print(string, color);
    newLine();
}

void println( 
        char character, 
        byte color) {
    print(character, color);
    newLine();
}

void println(
        byte number,
        byte color) {
    println((uint32_t)number, color);
}

void println(
        int number,
        byte color) {
    print(number, color);
    newLine();
}

void println(
        uint32_t number,
        byte color) {
    print(number, color);
    newLine();
}

void clearScreen() {
    uint16_t blank = 0x20 | WHITE << 8;

    volatile char* video = (volatile char*) VIDEO_POSITION;
    memset((uint16_t*)video, blank, 80 * 25);
    row = 0;
    column = 0;
    updateCursor(row, column);
}