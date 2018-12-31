#include <panic.h> // The panic API uses C linkage.

#include <common/text_terminal.hpp>
#include <drivers/pc/vga_text_display_device.hpp>
#include <logger.hpp>
#include <inout.h>
#include <halt.h>
#include <interrupt_asm.h>
#include <backtrace.hpp>

#include <cstdio>
#include <cstdarg>

// Prints a message to the screen and halts forever.
// This is only called in emergencies.
// Further, panic() provides a platform-agnostic interface though it relies on
// a platform-dependent implementation. This implementation is suitable for
// either a i386 or x86_64 PC system. It takes control of the VGA text display
// device and prints a message at the top of the console.
extern "C"
__attribute__((noreturn))
void panic(const char* fmt, ...)
{
    disable_interrupts();
    TRACE("Kernel is going to panic.");

    constexpr size_t PANIC_BUFFER_SIZE = 1024;
    char buffer[PANIC_BUFFER_SIZE];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    TRACE("message is \"%s\"", buffer);

    // If the panic happened early in the boot process then we may not have
    // a text terminal yet. In this case, panic() makes its own.
    VGATextDisplayDevice display;
    UnlockedTextTerminal terminal(display);
    backtrace(terminal);
    terminal.printf("PANIC: %s\n\n", buffer);

    halt_forever();
}
