#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_GDT_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_GDT_HPP

extern "C" {
void i386_lgdt(void *gdt, unsigned limit);
}

namespace i386 {
    inline void lgdt(void *gdt, unsigned limit)
    {
#       ifdef TESTING
        (void)gdt;
        (void)limit;
#       else
        i386_lgdt(gdt, limit);
#       endif
    }
} // namespace i386

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_GDT_HPP
