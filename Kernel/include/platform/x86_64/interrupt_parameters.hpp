#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_INTERRUPT_PARAMETERS_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_INTERRUPT_PARAMETERS_HPP

#include <cstdint>

namespace x86_64 {

// A parameter package constructed by the low-level ISR wrapper.
struct InterruptParameters {
    uint16_t fs;
    uint16_t gs;
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
    uint64_t interrupt_number;
    uint64_t size; // The size of the structure.
    uint64_t error_code;
    uint64_t rip;
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_INTERRUPT_PARAMETERS_HPP
