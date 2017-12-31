#pragma once

// Abstract interface for an object which handles an interrupt.
// This can be called by the interrupt dispatcher when the interrupt occurs.
class interrupt_handler {
public:
    virtual ~interrupt_handler() noexcept = default;
    virtual void int_handler() = 0;
};
