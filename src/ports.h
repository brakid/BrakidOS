#ifndef __PORTS__
#define __PORTS__
#include "types.h"
class Ports {
    public:
        static void port_byte_out(uint32_t port, uint8_t data);
        static uint8_t port_byte_in(uint32_t port);
};
#endif