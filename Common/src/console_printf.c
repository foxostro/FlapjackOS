#include "flapjack_libc/stdio.h"
#include "flapjack_libc/string.h"
#include "console_printf.h"

size_t console_printf(const console_interface_t *console, const char * restrict fmt, ...)
{
    char buffer[128] = {0};

    va_list args;
    va_start(args, fmt);
    VSNPRINTF(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    console->puts(buffer);

    return STRNLEN(buffer, sizeof(buffer));
}
