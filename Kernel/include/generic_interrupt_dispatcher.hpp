#ifndef FLAPJACKOS_KERNEL_INCLUDE_GENERIC_INTERRUPT_DISPATCHER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_GENERIC_INTERRUPT_DISPATCHER_HPP

#include <common/shared_pointer.hpp>
#include <common/lock_guard.hpp>
#include <common/interrupt_lock.hpp>
#include <panic.h>
#include <cstddef>
#include "generic_interrupt_handler.hpp"

// Invoked when an interrupt occurs to redirect to the appropriate handler.
template<typename HardwareInterruptController,
         typename Params,
         typename InterruptNamer>
class GenericInterruptDispatcher {
public:
    static constexpr size_t MAX_HANDLERS = 256;
    
    using LockType = InterruptLock;
    using Handler = SharedPointer<GenericInterruptHandler<Params>, LockType>;

    GenericInterruptDispatcher(HardwareInterruptController& hardware_interrupt_controller)
     : should_panic_on_null_handler_(true),
       hardware_interrupt_controller_(hardware_interrupt_controller)
    {}

    // Sets the handler for the specified interrupt number.
    // Pass a null handler to specify that no action should be taken.
    // The dispatcher does not take ownership of the handler object.
    void set_handler(unsigned interrupt_number, const Handler& handler)
    {
        assert(interrupt_number < MAX_HANDLERS);
        LockGuard lock{lock_};
        handlers_[interrupt_number] = handler;
    }

    // Gets the specified interrupt handler.
    Handler get_handler(unsigned interrupt_number)
    {
        LockGuard lock{lock_};
        Handler handler = handlers_[interrupt_number];
        return handler;
    }

    // Invoked when an interrupt occurs to redirect to the appropriate handler.
    void dispatch(const Params& params)
    {
        LockGuard lock{lock_};
        assert(params.size == sizeof(Params));
        unsigned interrupt_number = params.interrupt_number;
        assert(interrupt_number < MAX_HANDLERS);
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
    void set_should_panic_on_null_handler(bool should_panic)
    {
        LockGuard lock{lock_};
        should_panic_on_null_handler_ = should_panic;
    }

    bool should_panic_on_null_handler()
    {
        LockGuard lock{lock_};
        return should_panic_on_null_handler_;
    }

private:
    LockType lock_;
    Handler handlers_[MAX_HANDLERS];
    bool should_panic_on_null_handler_;
    HardwareInterruptController& hardware_interrupt_controller_;

    bool clear_hardware_interrupt(unsigned interrupt_number)
    {
        return hardware_interrupt_controller_.clear(interrupt_number);
    }

    void panic_on_null_handler(unsigned interrupt_number)
    {
        panic("No handler for interrupt \"%s\" (%u)\n",
            get_interrupt_name(interrupt_number),
            interrupt_number);
    }
    
    const char* get_interrupt_name(unsigned interrupt_number) const
    {
        return InterruptNamer::get_interrupt_name(interrupt_number);
    }
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_GENERIC_INTERRUPT_DISPATCHER_HPP
