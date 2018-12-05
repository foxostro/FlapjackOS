#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_CREG_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_CREG_HPP

extern "C" {
uint32_t i386_get_cr2(void);
uint32_t i386_get_cr3(void);
} // extern "C"

namespace i386 {
    inline uint32_t get_cr2()
    {
#       ifdef TESTING
    	return 0;
#       else
        return i386_get_cr2();
#       endif
    }

    inline uint32_t get_cr3()
    {
#       ifdef TESTING
    	return 0;
#       else
        return i386_get_cr3();
#       endif
    }
} // namespace i386

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_CREG_HPP
