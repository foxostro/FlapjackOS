#include <console_printf.hpp>

// Maximum number of stack frames in a backtrace.
constexpr size_t MAXFRAMES = 16;

extern "C" {
    unsigned* get_ebp(void); // defined in backtrace_asm.S
}

void backtrace(console_device &console)
{
    unsigned frame;
    unsigned * ebp;

    ebp = get_ebp();

    console_printf(console, "Back Trace:\n");
    for (frame = 0; frame < MAXFRAMES; ++frame) {
        unsigned eip;

        if(ebp == NULL) {
            // We set ebp to zero at the root stack frame.
            break;
        }

        eip = ebp[1];

        if(eip == 0) {
            // No return address?
            break;
        }

        ebp = (unsigned *)(ebp[0]);
        console_printf(console, "[%p]\n", eip);
    }

    console_printf(console, "\n");
}
