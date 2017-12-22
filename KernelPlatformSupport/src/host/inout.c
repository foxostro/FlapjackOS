#include <inout.h>

uint8_t inb(__attribute__((unused)) uint16_t port)
{
    return 0;
}

uint16_t inw(__attribute__((unused)) uint16_t port)
{
    return 0;
}

uint32_t ind(__attribute__((unused)) uint16_t port)
{
    return 0;
}

void outb(__attribute__((unused)) uint16_t port, __attribute__((unused)) uint8_t val)
{
    // do nothing
}

void outw(__attribute__((unused)) uint16_t port, __attribute__((unused)) uint16_t val)
{
    // do nothing
}

void outd(__attribute__((unused)) uint16_t port, __attribute__((unused)) uint32_t val)
{
    // do nothing
}
