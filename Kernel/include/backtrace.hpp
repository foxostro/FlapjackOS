#ifndef FLAPJACKOS_KERNEL_INCLUDE_BACKTRACE_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_BACKTRACE_HPP

#include <common/text_terminal.hpp>
#include <get_frame_pointer.h>

template<typename Function>
void enumerate_stack_frames(Function&& function)
{
    constexpr size_t MAXFRAMES = 16;
    size_t frame;
    uintptr_t* frame_pointer;

    frame_pointer = get_frame_pointer();

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
        function(return_address);
    }
}

void backtrace(UnlockedTextTerminal &term);

#endif // FLAPJACKOS_KERNEL_INCLUDE_BACKTRACE_HPP
