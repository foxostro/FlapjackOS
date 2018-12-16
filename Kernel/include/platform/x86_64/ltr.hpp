#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_LTR_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_LTR_HPP

extern "C" {
void x86_64_ltr(void);
}

namespace x86_64 {
    inline void ltr()
    {
#       ifndef TESTING
        x86_64_ltr();
#       endif
    }
} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_LTR_HPP
