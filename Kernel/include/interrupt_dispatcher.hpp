#ifndef FLAPJACKOS_KERNEL_INCLUDE_INTERRUPT_DISPATCHER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_INTERRUPT_DISPATCHER_HPP

#include <common/interrupt_handler.hpp>
#include <common/shared_pointer.hpp>
#include <idt_asm.h>
#include <interrupt_lock.hpp>

// Invoked when an interrupt occurs to redirect to the appropriate handler.
class InterruptDispatcher {
public:
    using LockType = InterruptLock;
    using Handler = SharedPointer<InterruptHandler, LockType>;
    using Params = InterruptHandler::ParameterPackage;

    InterruptDispatcher();

    // Sets the handler for the specified interrupt number.
    // Pass a null handler to specify that no action should be taken.
    // The dispatcher does not take ownership of the handler object.
    void set_handler(unsigned interrupt_number, const Handler& handler);

    // Gets the specified interrupt handler.
    Handler get_handler(unsigned interrupt_number);

    // Invoked when an interrupt occurs to redirect to the appropriate handler.
    void dispatch(unsigned interrupt_number, const Params& params) noexcept;

private:
    LockType lock_;
    Handler handlers_[IDT_MAX];
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_INTERRUPT_DISPATCHER_HPP