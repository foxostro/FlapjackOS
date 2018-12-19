#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_INTERRUPT_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_INTERRUPT_CONTROLLER_HPP

#include "hardware_interrupt_controller.hpp"
#include "interrupt_parameters.hpp"
#include "interrupt_dispatcher.hpp"

namespace x86_64 {

// AFOX_TODO: There's a lot of duplication between this class and the i386 version. Can these two be consolidated, maybe with a common base class?
class InterruptController {
public:
    InterruptController();

    // Initialize the underlying hardware interrupt controller.
    void initialize_hardware();

    // Installs interrupt handlers for system standard interrupts and traps.
    void setup();

    // Installs the specific handler for the given IRQ line.
    void install(unsigned irq, InterruptDispatcher::Handler handler);

    // Become ready to receive hardware interrupt and then enable them.
    void become_ready();

    // Disables interrupts.
    // If the interrupt controller is not ready then this does nothing.
    void disable_interrupts();

    // Enables interrupts.
    // If the interrupt controller is not ready then this does nothing.
    void enable_interrupts();

    // Redirect the interrupt to the appropriate handler.
    void dispatch(void* params);

private:
    HardwareInterruptController hardware_interrupt_controller_;
    InterruptDispatcher interrupt_dispatcher_;
    bool are_interrupts_ready_;
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_INTERRUPT_CONTROLLER_HPP
