#include <stdbool.h>
#include <console.h>
#include <kprintf.h>
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
    kvsnprintf(buffer, sizeof(buffer), fmt, args);
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
    kprintf(&g_console, "Registers:\n");
    kprintf(&g_console, "edi = 0x%x\n", edi);
    kprintf(&g_console, "esi = 0x%x\n", esi);
    kprintf(&g_console, "ebp = 0x%x\n", ebp);
    kprintf(&g_console, "esp = 0x%x\n", esp);
    kprintf(&g_console, "ebx = 0x%x\n", ebx);
    kprintf(&g_console, "edx = 0x%x\n", edx);
    kprintf(&g_console, "ecx = 0x%x\n", ecx);
    kprintf(&g_console, "eax = 0x%x\n", eax);
    kprintf(&g_console, "eip = %p\n\n", eip);

    backtrace(&g_console);

    if (error_code_present) {
        kprintf(&g_console, "Error Code: 0x%x\n", error_code);
    }

    panic("%s", message);
}
