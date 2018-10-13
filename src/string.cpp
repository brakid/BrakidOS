#include "string.h"

byte strlen(const char* string) {
    byte lenghtOfString = 0;
     
    while(*string++ != '\0') {
        lenghtOfString++;
    }
    return lenghtOfString;
}