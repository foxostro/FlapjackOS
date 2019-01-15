#include <drivers/pc/bochs_e9_hack.hpp>
#include <inout.h>

void BochsE9Hack::putchar(char ch)
{
    outb(BOCHS_LOGGER_PORT, ch);
}

void BochsE9Hack::puts(const char* s)
{
    while (*s) {
        putchar(*s++);
    }
    putchar('\n');
}
