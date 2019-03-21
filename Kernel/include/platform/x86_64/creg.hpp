#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_CREG_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_CREG_HPP

#include <cstdint>
#include <cassert>

extern "C" {
uint64_t x86_64_get_cr0(void);
void x86_64_set_cr0(uint64_t value);
uint64_t x86_64_get_cr2(void);
uint64_t x86_64_get_cr3(void);
void x86_64_set_cr3(uint64_t value);
uint64_t x86_64_get_cr4(void);
void x86_64_set_cr4(uint64_t value);
} // extern "C"

namespace x86_64 {
    inline uint64_t get_cr0()
    {
#       ifdef TESTING
          return 0;
#       else
        return x86_64_get_cr0();
#       endif
    }

    inline void set_cr0(uint64_t value)
    {
#       ifdef TESTING
        (void)value;
#       else
        return x86_64_set_cr0(value);
#       endif
    }

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

    inline void set_cr3(uint64_t value)
    {
#       ifdef TESTING
        (void)value;
#       else
        return x86_64_set_cr3(value);
#       endif
    }

    inline uint64_t get_cr4()
    {
#       ifdef TESTING
          return 0;
#       else
        return x86_64_get_cr4();
#       endif
    }

    inline void set_cr4(uint64_t value)
    {
#       ifdef TESTING
        (void)value;
#       else
        return x86_64_set_cr4(value);
#       endif
    }
} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_CREG_HPP
