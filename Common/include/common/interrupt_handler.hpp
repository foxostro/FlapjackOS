#ifndef FLAPJACKOS_COMMON_INCLUDE_INTERRUPT_HANDLER_HPP
#define FLAPJACKOS_COMMON_INCLUDE_INTERRUPT_HANDLER_HPP

// Abstract interface for an object which handles an interrupt.
// This can be called by the interrupt dispatcher when the interrupt occurs.
class InterruptHandler {
public:
    struct ParameterPackage {
        unsigned edi;
        unsigned esi;
        unsigned ebp;
        unsigned esp;
        unsigned ebx;
        unsigned edx;
        unsigned ecx;
        unsigned eax;
        unsigned error_code;
        unsigned eip;
    };

    virtual ~InterruptHandler() noexcept = default;

    virtual void int_handler(const ParameterPackage& params) = 0;
};

#endif // FLAPJACKOS_COMMON_INCLUDE_INTERRUPT_HANDLER_HPP
