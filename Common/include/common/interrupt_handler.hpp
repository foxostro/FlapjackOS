#ifndef FLAPJACKOS_COMMON_INCLUDE_INTERRUPT_HANDLER_HPP
#define FLAPJACKOS_COMMON_INCLUDE_INTERRUPT_HANDLER_HPP

// Abstract interface for an object which handles an interrupt.
// This can be called by the interrupt dispatcher when the interrupt occurs.
class InterruptHandler {
public:
    virtual ~InterruptHandler() noexcept = default;
    virtual void int_handler() = 0;
};

#endif // FLAPJACKOS_COMMON_INCLUDE_INTERRUPT_HANDLER_HPP
