#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_LIDT_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_LIDT_HPP

extern "C" {
void i386_lidt(void* idt, unsigned limit);
}

namespace i386 {
    inline void lidt([[maybe_unused]] void* idt,
                     [[maybe_unused]] unsigned limit)
    {
#       ifndef TESTING
        return i386_lidt(idt, limit);
#       endif
    }
} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_LIDT_HPP
