#include <common/console_printf.hpp>
#include <get_ebp.h>

// Maximum number of stack frames in a backtrace.
constexpr size_t MAXFRAMES = 16;

void backtrace(console_device &console)
{
    size_t frame;
    uintptr_t * ebp;

    ebp = get_ebp();

    console_printf(console, "Back Trace:\n");
    for (frame = 0; frame < MAXFRAMES; ++frame) {
        uintptr_t eip;

        if(ebp == nullptr) {
            // We set ebp to zero at the root stack frame.
            break;
        }

        eip = ebp[1];

        if(eip == 0) {
            // No return address?
            break;
        }

        ebp = (uintptr_t *)(ebp[0]);
        console_printf(console, "[%p]\n", eip);
    }

    console_printf(console, "\n");
}
