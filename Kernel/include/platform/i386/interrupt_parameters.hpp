#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_INTERRUPT_PARAMETERS_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_INTERRUPT_PARAMETERS_HPP

#include <cstdint>

namespace i386 {

// A parameter package constructed by the low-level ISR wrapper.
struct InterruptParameters {
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

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_INTERRUPT_PARAMETERS_HPP
