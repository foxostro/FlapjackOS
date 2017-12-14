#include <stdarg.h>
#include <string.h>
#include <console.h>
#include <kprintf.h>

size_t kprintf(const console_interface_t *console, const char * restrict fmt, ...)
{
    char buffer[128] = {0};

    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    console->puts(buffer);

    return strnlen(buffer, sizeof(buffer));
}
