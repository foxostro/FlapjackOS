#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_INTERRUPT_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_INTERRUPT_CONTROLLER_HPP

#include "hardware_interrupt_controller.hpp"
#include "interrupt_parameters.hpp"
#include <generic_interrupt_dispatcher.hpp>

namespace i386 {

class InterruptController {
public:
    using InterruptDispatcher = GenericInterruptDispatcher<HardwareInterruptController, InterruptParameters>;

    InterruptController();

    // Initialize the underlying hardware interrupt controller.
    void init();

    // Installs interrupt handlers for system standard interrupts and traps.
    // InterruptController takes ownership of the handler.
    // AFOX_TODO: I should implement my own equivalent to std::unique_ptr.
    void install(GenericInterruptHandler<InterruptParameters> *keyboard_handler);

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

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_INTERRUPT_CONTROLLER_HPP
