#include <stdbool.h>
#include <stdio.h>
#include <console.h>
#include <console_printf.h>
#include <halt.h>
#include <interrupt_asm.h>
#include <backtrace.h>

extern console_interface_t g_console; // defined in kernel.c

__attribute__((noreturn))
void panic(const char * restrict fmt, ...)
{
    disable_interrupts();

    char buffer[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    g_console.puts("PANIC: ");
    g_console.puts(buffer);
    g_console.puts("\n");

    halt_forever();
}

__attribute__((noreturn))
void panic2(const char * restrict message,
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
    console_printf(&g_console, "Registers:\n");
    console_printf(&g_console, "edi = 0x%x\n", edi);
    console_printf(&g_console, "esi = 0x%x\n", esi);
    console_printf(&g_console, "ebp = 0x%x\n", ebp);
    console_printf(&g_console, "esp = 0x%x\n", esp);
    console_printf(&g_console, "ebx = 0x%x\n", ebx);
    console_printf(&g_console, "edx = 0x%x\n", edx);
    console_printf(&g_console, "ecx = 0x%x\n", ecx);
    console_printf(&g_console, "eax = 0x%x\n", eax);
    console_printf(&g_console, "eip = %p\n\n", eip);

    backtrace(&g_console);

    if (error_code_present) {
        console_printf(&g_console, "Error Code: 0x%x\n", error_code);
    }

    panic("%s", message);
}
