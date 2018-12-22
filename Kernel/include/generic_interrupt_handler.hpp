#ifndef FLAPJACKOS_KERNEL_INCLUDE_GENERIC_INTERRUPT_HANDLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_GENERIC_INTERRUPT_HANDLER_HPP

#include <cstdint>

// Abstract interface for an object which handles an interrupt.
// This can be called by the interrupt dispatcher when the interrupt occurs.
template<typename ParameterPackage>
class GenericInterruptHandler {
public:

    virtual ~GenericInterruptHandler() = default;

    virtual void int_handler(const ParameterPackage& params) = 0;
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_GENERIC_INTERRUPT_HANDLER_HPP
