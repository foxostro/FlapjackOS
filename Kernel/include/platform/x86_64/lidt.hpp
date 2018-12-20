#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_LIDT_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_LIDT_HPP

extern "C" {
void x86_64_lidt(void* idt, unsigned limit);
}

namespace x86_64 {
    inline void lidt([[maybe_unused]] void* idt,
                     [[maybe_unused]] unsigned limit)
    {
#       ifndef TESTING
        return x86_64_lidt(idt, limit);
#       endif
    }
} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_LIDT_HPP
