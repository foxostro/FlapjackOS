#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "console_printf.h"

size_t console_printf(const console_interface_t *console, const char *fmt, ...)
{
    char buffer[128] = {0};

    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    console->puts(buffer);

    return strnlen(buffer, sizeof(buffer));
}
