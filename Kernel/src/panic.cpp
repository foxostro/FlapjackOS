#include <panic.h> // The panic API uses C linkage.

#include <common/text_terminal.hpp>
#include <drivers/pc/vga_text_display_device.hpp> // TODO: How can I make panic more platform-agnostic?
#include <logger.hpp>
#include <inout.h>
#include <halt.h>
#include <interrupt_asm.h>
#include <backtrace.hpp>

#include <cstdio>
#include <cstdarg>

extern TextTerminal *g_terminal; // defined in kernel.cpp

constexpr size_t PANIC_BUFFER_SIZE = 1024;

extern "C"
__attribute__((noreturn))
void panic(const char *fmt, ...)
{
    TRACE("Kernel is going to panic.");

    disable_interrupts();

    char buffer[PANIC_BUFFER_SIZE];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    TRACE("message is \"%s\"", buffer);

    if (g_terminal) {
        g_terminal->printf("PANIC: %s\n", buffer);
    } else {
        // If the panic happened early in the boot process then we may not have
        // a text terminal yet. In this case, panic() makes its own.
        VGATextDisplayDevice display;
        TextTerminal terminal;
        terminal.init(&display);
        terminal.printf("PANIC: %s\n", buffer);
    }

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
    TextTerminal &term = *g_terminal;

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
