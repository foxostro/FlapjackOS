#include <console.h>
#include <kprintf.h>
#include <halt.h>
#include <interrupt_asm.h>

__attribute__((noreturn))
void panic(const char * restrict fmt, ...)
{
    disable_interrupts();

    char buffer[128];
    va_list args;
    va_start(args, fmt);
    kvsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    console_puts("PANIC: ");
    console_puts(buffer);
    console_puts("\n");

    halt();
}
