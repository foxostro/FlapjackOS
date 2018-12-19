#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_INTERRUPT_PARAMETERS_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_INTERRUPT_PARAMETERS_HPP

#include <cstdint>

namespace x86_64 {

// A parameter package constructed by the low-level ISR wrapper.
struct InterruptParameters {
    uint32_t size; // The size of the structure.
    // STUB
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_INTERRUPT_PARAMETERS_HPP
