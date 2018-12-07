#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_CREG_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_CREG_HPP

#include <cstdint>
#include <cassert>

extern "C" {
uint64_t x86_64_get_cr2(void);
uint64_t x86_64_get_cr3(void);
void x86_64_set_cr2(uint64_t value);
void x86_64_set_cr3(uint64_t value);
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
        return x86_64_get_cr3();
#       endif
    }

    inline void set_cr2(uint64_t value)
    {
        (void)value;
        assert(!"set_cr2 is a stub");
    }

    inline void set_cr3(uint64_t value)
    {
        (void)value;
        assert(!"set_cr3 is a stub");
    }
} // namespace x86_64

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_CREG_HPP
