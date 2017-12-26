#include <cstdio>
#include <cstdarg>
#include <common/text_terminal.hpp>
#include <halt.h>
#include <interrupt_asm.h>
#include <backtrace.hpp>
#include <panic.h> // The panic API uses C linkage.

extern text_terminal *g_terminal; // defined in kernel.c

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

    g_terminal->puts("PANIC: ");
    g_terminal->puts(buffer);
    g_terminal->puts("\n");

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
    text_terminal &term = *g_terminal;

    term.printf("Registers:\n");
    term.printf("edi = 0x%x\n", edi);
    term.printf("esi = 0x%x\n", esi);
    term.printf("ebp = 0x%x\n", ebp);
    term.printf("esp = 0x%x\n", esp);
    term.printf("ebx = 0x%x\n", ebx);
    term.printf("edx = 0x%x\n", edx);
    term.printf("ecx = 0x%x\n", ecx);
    term.printf("eax = 0x%x\n", eax);
    term.printf("eip = %p\n\n", eip);

    backtrace(term);

    if (error_code_present) {
        term.printf("Error Code: 0x%x\n", error_code);
    }

    panic("%s", message);
}
