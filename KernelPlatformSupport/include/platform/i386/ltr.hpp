#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_LTR_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_LTR_HPP

extern "C" {
void i386_ltr(void);
}

namespace i386 {
    inline void ltr()
    {
#       ifndef TESTING
        i386_ltr();
#       endif
    }
} // namespace i386

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_LTR_HPP
