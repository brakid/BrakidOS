#include "global.h"
#include "io.h"
#include "string.h"
#include "ports.h"
#include "utils.h"

byte row = 0;
byte column = 0;

void update_cursor(byte row, byte column){
    int position = (row * COLUMNS + column);
 
    Ports::port_byte_out(VGA_INDEX_REGISTER, 0x0F);
    Ports::port_byte_out(VGA_DATA_REGISTER, (uint8_t) (position & 0xFF));
    Ports::port_byte_out(VGA_INDEX_REGISTER, 0x0E);
    Ports::port_byte_out(VGA_DATA_REGISTER, (uint8_t) ((position >> 8) & 0xFF));
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

void Io::print( 
        const char* string, 
        byte color) {
    printOffset(row, column, string, color);
}

void Io::print( 
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

void Io::print(
        byte number,
        byte color) {
    Io::print((uint32_t)number, color);
}

void Io::print(
        uint32_t number,
        byte color) {
    uint32_t compare = 1000000000;
    bool hasPrinted = false;
    while (compare > 0) {
        byte digit = number / compare;
        number -= digit * compare;
        compare /= 10;
        if (digit != 0 || hasPrinted == true || compare == 0) {
            Io::print(Utils::digitToChar(digit), color);
            hasPrinted = true;
        }
    }
}

void Io::println( 
        const char* string, 
        byte color) {
    printOffset(row, column, string, color);
    row++;
    column = 0;
    update_cursor(row, column);
}

void Io::println( 
        char character, 
        byte color) {
    Io::print(character, color);
    row++;
    column = 0;
    update_cursor(row, column);
}

void Io::println(
        byte number,
        byte color) {
    Io::println((uint32_t)number, color);
}

void Io::println(
        uint32_t number,
        byte color) {
    Io::print(number, color);
    row++;
    column = 0;
    update_cursor(row, column);
}

void Io::clearScreen() {
    volatile char* video = (volatile char*) VIDEO_POSITION;
    int count = 0;
    while(count < ROWS * COLUMNS) {
        *video++ = ' ';
        *video++ = WHITE;
        count++;
    }
    row = 0;
    column = 0;
    update_cursor(row, column);
}