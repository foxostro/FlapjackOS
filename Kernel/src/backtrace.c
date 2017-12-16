#include <console_printf.h>

// Maximum number of stack frames in a backtrace.
#define MAXFRAMES (16)

unsigned* get_ebp(void); // defined in backtrace_asm.S

void backtrace(const console_interface_t *console)
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
