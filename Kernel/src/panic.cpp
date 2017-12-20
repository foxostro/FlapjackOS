#include <cstdio>
#include <common/console_device.hpp>
#include <common/console_printf.hpp>
#include <halt.h>
#include <interrupt_asm.h>
#include <backtrace.hpp>
#include <panic.h> // The panic API uses C linkage.

extern console_device *g_console; // defined in kernel.c

extern "C"
__attribute__((noreturn))
void panic(const char *fmt, ...)
{
    disable_interrupts();

    char buffer[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    g_console->puts("PANIC: ");
    g_console->puts(buffer);
    g_console->puts("\n");

    halt_forever();
}

extern "C"
__attribute__((noreturn))
void panic2(const char *message,
            unsigned edi,
            unsigned esi,
            unsigned ebp,
            unsigned esp,
            unsigned ebx,
            unsigned edx,
            unsigned ecx,
            unsigned eax,
            bool error_code_present,
            unsigned error_code,
            unsigned eip)
{
    console_device &console = *g_console;

    console_printf(console, "Registers:\n");
    console_printf(console, "edi = 0x%x\n", edi);
    console_printf(console, "esi = 0x%x\n", esi);
    console_printf(console, "ebp = 0x%x\n", ebp);
    console_printf(console, "esp = 0x%x\n", esp);
    console_printf(console, "ebx = 0x%x\n", ebx);
    console_printf(console, "edx = 0x%x\n", edx);
    console_printf(console, "ecx = 0x%x\n", ecx);
    console_printf(console, "eax = 0x%x\n", eax);
    console_printf(console, "eip = %p\n\n", eip);

    backtrace(console);

    if (error_code_present) {
        console_printf(console, "Error Code: 0x%x\n", error_code);
    }

    panic("%s", message);
}
