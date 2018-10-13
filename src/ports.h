#ifndef __PORTS__
#define __PORTS__
#include "types.h"

void port_byte_out(uint32_t port, uint8_t data);
uint8_t port_byte_in(uint32_t port);
#endif