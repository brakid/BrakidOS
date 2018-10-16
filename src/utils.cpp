#include "utils.h"
#include "io.h"

uint32_t criticalCount = 0;

char digitToChar(byte digit) {
    switch(digit) {
        case 1: return '1';
        case 2: return '2';
        case 3: return '3';
        case 4: return '4';
        case 5: return '5';
        case 6: return '6';
        case 7: return '7';
        case 8: return '8';
        case 9: return '9';
        case 0: return '0';
        default: return ' ';
    }
}

int ctoi(char character) {
    switch(character) {
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        case '0': return 0;
        default: return -1;
    }
}

int atoi(char* string) {
    int number = 0;
    int multiplier = 1;
    // check first character: -, +
    if(*string == '-') {
        multiplier = -1;
        string++;
    } else if (*string == '+') {
        // skip
        string++;
    }
    while(*string != 0) {
        int digit = ctoi(*string);
        if (digit == -1) {
            print("Input string is not a number: ");
            println(string);
            return -1;
        }
        string++;
        number = number * 10 + digit;
    }

    return number * multiplier;
}

byte* memcpy(byte* dest, const byte* src, uint32_t count) {
    while (count-- > 0) {
        *dest++ = *src++;
    }
    return dest;
}

uint16_t* memcpy(uint16_t* dest, const uint16_t* src, uint32_t count) {
    while (count-- > 0) {
        *dest++ = *src++;
    }
    return dest;
}
        
byte* memset(byte* dest, byte val, uint32_t count) {
    while (count-- > 0) {
        *dest++ = val;
    }
    return dest;
}

uint16_t* memset(uint16_t* dest, uint16_t val, uint32_t count) {
    while (count-- > 0) {
        *dest++ = val;
    }
    return dest;
}
        
uint32_t* memset(uint32_t* dest, uint32_t val, uint32_t count) {
    while (count-- > 0) {
        *dest++ = val;
    }
    return dest;
}

void disableInterupts() {
    asm("cli");
}

void enableInterupts() {
    asm("sti");
}

void enterCritical() {
    disableInterupts();
    criticalCount++;
}

void leaveCritical() {
    criticalCount--;
    if (criticalCount == 0) {
        enableInterupts();
    }
}