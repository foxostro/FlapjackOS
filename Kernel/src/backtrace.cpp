#include <backtrace.hpp>
#include <get_frame_pointer.h>

// Maximum number of stack frames in a backtrace.
constexpr size_t MAXFRAMES = 16;

void backtrace(TextTerminal &term)
{
    size_t frame;
    uintptr_t* frame_pointer;

    frame_pointer = get_frame_pointer();

    term.printf("Back Trace:\n");
    for (frame = 0; frame < MAXFRAMES; ++frame) {
        uintptr_t return_address;

        if(frame_pointer == nullptr) {
            // We set ebp to zero at the root stack frame.
            break;
        }

        return_address = frame_pointer[1];

        if(return_address == 0) {
            // No return address?
            break;
        }

        frame_pointer = (uintptr_t *)(frame_pointer[0]);
        term.printf("[%p]\n", return_address);
    }

    term.printf("\n");
}
