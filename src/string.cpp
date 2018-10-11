#include "string.h"

byte String::strlen(const char* string) {
    byte lenghtOfString = 0;
     
    while(*string++ != '\0') {
        lenghtOfString++;
    }
    return lenghtOfString;
}