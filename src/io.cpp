#include "global.h"
#include "io.h"
#include "string.h"
#include "ports.h"
#include "utils.h"

byte row = 0;
byte column = 0;

void update_cursor(byte row, byte column){
    int position = (row * COLUMNS + column);
 
    port_byte_out(VGA_INDEX_REGISTER, 0x0F);
    port_byte_out(VGA_DATA_REGISTER, (uint8_t) (position & 0xFF));
    port_byte_out(VGA_INDEX_REGISTER, 0x0E);
    port_byte_out(VGA_DATA_REGISTER, (uint8_t) ((position >> 8) & 0xFF));
}

void new_line() {
    row++;
    column = 0;
    update_cursor(row, column);
}

void printOffset(
        byte rowOffset,
        byte columnOffset,
        const char* string, 
        byte color) {
    row = rowOffset;
    column = columnOffset;

    volatile char* video = (volatile char*) VIDEO_POSITION;
    video += (row * COLUMNS + column) * sizeof(char) * 2;
    while(*string != 0) {
        *video++ = *string++;
        *video++ = color;
        column++;
    }
    
    if (column > COLUMNS) {
        byte rows = column / COLUMNS;
        column -= rows * COLUMNS;
        row += rows; 
    }
    if (row > ROWS) {
        row = ROWS;
    }
    update_cursor(row, column);
}

void print( 
        const char* string, 
        byte color) {
    printOffset(row, column, string, color);
}

void print( 
        char character, 
        byte color) {
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
    if (row > ROWS) {
        row = ROWS;
    }
    update_cursor(row, column);
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
    uint32_t compare = 1000000000;
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
    printOffset(row, column, string, color);
    new_line();
}

void println( 
        char character, 
        byte color) {
    print(character, color);
    new_line();
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
    new_line();
}

void println(
        uint32_t number,
        byte color) {
    print(number, color);
    new_line();
}

void clearScreen() {
    uint16_t blank = 0x20 | WHITE << 8;

    volatile char* video = (volatile char*) VIDEO_POSITION;
    memset((uint16_t*)video, blank, 80 * 25);
    row = 0;
    column = 0;
    update_cursor(row, column);
}