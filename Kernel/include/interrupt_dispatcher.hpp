#ifndef FLAPJACKOS_KERNEL_INCLUDE_INTERRUPT_DISPATCHER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_INTERRUPT_DISPATCHER_HPP

#include <idt_asm.h>
#include <common/interrupt_handler.hpp>

// Invoked when an interrupt occurs to redirect to the appropriate handler.
class InterruptDispatcher {
public:
    InterruptDispatcher();

    // Sets the handler for the specified interrupt number.
    // Pass a null handler to specify that no action should be taken.
    // The dispatcher does not take ownership of the handler object.
    // TODO: Add locking to protect 'handler_' while interrupts are enabled.
    void set_handler(unsigned interrupt_number, InterruptHandler* handler);

    // Invoked when an interrupt occurs to redirect to the appropriate handler.
    using Params = InterruptHandler::ParameterPackage;
    void dispatch(unsigned interrupt_number, const Params& params) noexcept;

private:
    InterruptHandler* handlers_[IDT_MAX];
};

// The global interrupt dispatcher is invoked by the low-level interrupt
// handler routine. There is only one PIC and one IDT so there can only be one
// dispatcher.
extern InterruptDispatcher g_interrupt_dispatcher;

#endif // FLAPJACKOS_KERNEL_INCLUDE_INTERRUPT_DISPATCHER_HPP
