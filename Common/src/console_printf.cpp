#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <interfaces/console_device.hpp>
#include "console_printf.hpp"

size_t console_printf(console_device &console, const char *fmt, ...)
{
    char buffer[128] = {0};

    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    console.puts(buffer);

    return strnlen(buffer, sizeof(buffer));
}
