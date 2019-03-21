#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_CREG_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_CREG_HPP

#include <cstdint> // for uint32_t

extern "C" {
uint32_t i386_get_cr0(void);
void i386_set_cr0(uint32_t value);
uint32_t i386_get_cr2(void);
uint32_t i386_get_cr3(void);
void i386_set_cr3(uint32_t value);
uint32_t i386_get_cr4(void);
void i386_set_cr4(uint32_t value);
} // extern "C"

namespace i386 {
    inline uint32_t get_cr0()
    {
#       ifdef TESTING
    	return 0;
#       else
        return i386_get_cr0();
#       endif
    }

    inline void set_cr0(uint32_t value)
    {
#       ifdef TESTING
    	(void)value;
#       else
        return i386_set_cr0(value);
#       endif
    }

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

    inline void set_cr3(uint32_t value)
    {
#       ifdef TESTING
    	(void)value;
#       else
        return i386_set_cr3(value);
#       endif
    }

    inline uint32_t get_cr4()
    {
#       ifdef TESTING
    	return 0;
#       else
        return i386_get_cr4();
#       endif
    }

    inline void set_cr4(uint32_t value)
    {
#       ifdef TESTING
    	(void)value;
#       else
        return i386_set_cr4(value);
#       endif
    }
} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_CREG_HPP
