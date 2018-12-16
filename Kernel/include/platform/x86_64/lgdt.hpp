#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_GDT_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_GDT_HPP

extern "C" {
void x86_64_lgdt(void *gdt, unsigned limit);
}

namespace x86_64 {
    inline void lgdt(void *gdt, unsigned limit)
    {
#       ifdef TESTING
        (void)gdt;
        (void)limit;
#       else
        x86_64_lgdt(gdt, limit);
#       endif
    }
} // namespace x86_64

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_GDT_HPP
