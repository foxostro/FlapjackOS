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

constexpr size_t PANIC_BUFFER_SIZE = 1024;

extern "C"
__attribute__((noreturn))
void panic(const char *fmt, ...)
{
    disable_interrupts();
    TRACE("Kernel is going to panic.");

    char buffer[PANIC_BUFFER_SIZE];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    TRACE("message is \"%s\"", buffer);

    // If the panic happened early in the boot process then we may not have
    // a text terminal yet. In this case, panic() makes its own.
    VGATextDisplayDevice display;
    UnlockedTextTerminal terminal;
    terminal.init(&display);
    terminal.printf("PANIC: %s\n\n", buffer);

    halt_forever();
}
