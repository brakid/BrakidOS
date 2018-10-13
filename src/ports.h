#ifndef __PORTS__
#define __PORTS__
#include "types.h"

#define PIC_1_CTRL 0x20
#define PIC_2_CTRL 0xA0
#define PIC_1_DATA 0x21
#define PIC_2_DATA 0xA1

void port_byte_out(uint32_t port, uint8_t data);
uint8_t port_byte_in(uint32_t port);
#endif