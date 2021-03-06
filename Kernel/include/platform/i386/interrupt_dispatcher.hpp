#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_INTERRUPT_DISPATCHER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_INTERRUPT_DISPATCHER_HPP

#include "hardware_interrupt_controller.hpp"
#include "interrupt_parameters.hpp"
#include <generic_interrupt_dispatcher.hpp>
#include <platform/i386/interrupt_numbers.hpp>

namespace i386 {

// Function object. Gets the string name of a given interrupt.
struct InterruptNamer {
    static const char* get_interrupt_name(unsigned interrupt_number)
    {
        switch (interrupt_number) {
            case IDT_KEY:   return "Keyboard";
            case IDT_TIMER: return "Timer";
            case IDT_DE:    return "Division Error";
            case IDT_DB:    return "Debug Exception";
            case IDT_NMI:   return "Non-Maskable Interrupt";
            case IDT_BP:    return "Breakpoint";
            case IDT_OF:    return "Overflow";
            case IDT_BR:    return "BOUND Range exceeded";
            case IDT_UD:    return "Undefined Opcode";
            case IDT_NM:    return "No Math coprocessor";
            case IDT_DF:    return "Double Fault.";
            case IDT_CSO:   return "Coprocessor Segment Overrun";
            case IDT_TS:    return "Invalid Task Segment Selector.";
            case IDT_NP:    return "Segment Not Present.";
            case IDT_SS:    return "Stack Segment Fault.";
            case IDT_GP:    return "General Protection Fault.";
            case IDT_PF:    return "Page Fault";
            case IDT_MF:    return "X87 Math Fault";
            case IDT_AC:    return "Alignment Check";
            case IDT_MC:    return "Machine Check";
            case IDT_XF:    return "SSE Floating Point Exception";
            default:        return "unknown";
        };
    }
};

using InterruptDispatcher = GenericInterruptDispatcher<HardwareInterruptController,
                                                       InterruptParameters,
                                                       InterruptNamer>;

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_INTERRUPT_DISPATCHER_HPP
