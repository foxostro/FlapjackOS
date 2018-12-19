#ifndef FLAPJACKOS_KERNEL_INCLUDE_GENERIC_INTERRUPT_DISPATCHER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_GENERIC_INTERRUPT_DISPATCHER_HPP

#include <common/shared_pointer.hpp>
#include <idt_asm.h>
#include <interrupt_lock.hpp>
#include <panic.h>
#include "generic_interrupt_handler.hpp"

// Invoked when an interrupt occurs to redirect to the appropriate handler.
template<typename HardwareInterruptController, typename Params>
class GenericInterruptDispatcher {
public:
    using LockType = InterruptLock;
    using Handler = SharedPointer<GenericInterruptHandler<Params>, LockType>;

    GenericInterruptDispatcher(HardwareInterruptController& hardware_interrupt_controller)
     : should_panic_on_null_handler_(true),
       hardware_interrupt_controller_(hardware_interrupt_controller)
    {}

    // Sets the handler for the specified interrupt number.
    // Pass a null handler to specify that no action should be taken.
    // The dispatcher does not take ownership of the handler object.
    void set_handler(unsigned interrupt_number, const Handler& handler) noexcept
    {
        assert(interrupt_number < IDT_MAX);
        lock_.lock();
        handlers_[interrupt_number] = handler;
        lock_.unlock();
    }

    // Gets the specified interrupt handler.
    Handler get_handler(unsigned interrupt_number) noexcept
    {
        lock_.lock();
        Handler handler = handlers_[interrupt_number];
        lock_.unlock();
        return handler;
    }

    // Invoked when an interrupt occurs to redirect to the appropriate handler.
    void dispatch(const Params& params) noexcept
    {
        assert(params.size == sizeof(Params));
        unsigned interrupt_number = params.interrupt_number;
        assert(interrupt_number < IDT_MAX);
        bool spurious = clear_hardware_interrupt(interrupt_number);
        if (!spurious) {
            auto handler = get_handler(interrupt_number);
            if (handler) {
                handler->int_handler(params);
            } else if (should_panic_on_null_handler()) {
                panic_on_null_handler(interrupt_number);
            }
        }
    }

    // Indicates that the kernel should panic if the handler is null.
    // This is useful for handling faults early in the boot process, before the
    // memory allocators are available.
    void set_should_panic_on_null_handler(bool should_panic) noexcept
    {
        lock_.lock();
        should_panic_on_null_handler_ = should_panic;
        lock_.unlock();
    }

    bool should_panic_on_null_handler() noexcept
    {
        bool should_panic; 
        lock_.lock();
        should_panic = should_panic_on_null_handler_;
        lock_.unlock();
        return should_panic;
    }

protected:
    virtual const char* get_interrupt_name(unsigned interrupt_number) = 0;

private:
    LockType lock_;
    Handler handlers_[IDT_MAX];
    bool should_panic_on_null_handler_;
    HardwareInterruptController& hardware_interrupt_controller_;

    bool clear_hardware_interrupt(unsigned interrupt_number) noexcept
    {
        return hardware_interrupt_controller_.clear(interrupt_number);
    }

    void panic_on_null_handler(unsigned interrupt_number) noexcept
    {
        panic("No handler for interrupt \"%s\" (%u)\n",
            get_interrupt_name(interrupt_number),
            interrupt_number);
    }
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_GENERIC_INTERRUPT_DISPATCHER_HPP
