#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_INTERRUPT_PARAMETERS_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_INTERRUPT_PARAMETERS_HPP

#include <cstdint>

namespace x86_64 {

// A parameter package constructed by the low-level ISR wrapper.
struct InterruptParameters {
    uint64_t size; // The size of the structure.
    uint64_t interrupt_number;
    uint64_t rip;
    uint64_t error_code;
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rbp;
    uint64_t rsp;
    uint64_t rbx;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rax;
    char fxsave[512];
};

static_assert(672 == sizeof(InterruptParameters),
              "The assembly wrappers onstruct a 672 byte structure in memory.");

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_INTERRUPT_PARAMETERS_HPP
