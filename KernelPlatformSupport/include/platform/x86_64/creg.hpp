#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_CREG_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_CREG_HPP

extern "C" {
uint64_t x86_64_get_cr2(void);
uint64_t x86_64_get_cr2(void);
} // extern "C"

namespace x86_64 {
    inline uint64_t get_cr2()
    {
#       ifdef TESTING
    	return 0;
#       else
        return x86_64_get_cr2();
#       endif
    }

    inline uint64_t get_cr3()
    {
#       ifdef TESTING
    	return 0;
#       else
        return x86_64_get_cr2();
#       endif
    }
} // namespace x86_64

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_CREG_HPP
