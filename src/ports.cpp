#include "ports.h"

void port_byte_out(uint32_t port, uint8_t data) {
    // "a" (data) means: load EAX with data
    // "d" (port) means: load EDX with port
    asm("out dx, al" : : "a" (data), "d" (port));
}

uint8_t port_byte_in(uint32_t port) {
    uint8_t data;
    // "=a" (data) means: put AL into data
    // "d" (port) means: load EDX with port
    asm("in al, dx" : "=a" (data) : "d" (port));
    return data;
}