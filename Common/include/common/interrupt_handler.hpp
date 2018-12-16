#ifndef FLAPJACKOS_COMMON_INCLUDE_INTERRUPT_HANDLER_HPP
#define FLAPJACKOS_COMMON_INCLUDE_INTERRUPT_HANDLER_HPP

#include <cstdint>

// Abstract interface for an object which handles an interrupt.
// This can be called by the interrupt dispatcher when the interrupt occurs.
class InterruptHandler {
public:
    struct ParameterPackage {
        uint32_t size; // The size of the structure.
        uint32_t interrupt_number;
        uint32_t edi;
        uint32_t esi;
        uint32_t ebp;
        uint32_t esp;
        uint32_t ebx;
        uint32_t edx;
        uint32_t ecx;
        uint32_t eax;
        uint32_t error_code;
        uint32_t eip;
    };

    virtual ~InterruptHandler() noexcept = default;

    virtual void int_handler(const ParameterPackage& params) = 0;
};

#endif // FLAPJACKOS_COMMON_INCLUDE_INTERRUPT_HANDLER_HPP
