#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_ABSTRACT_FLOATING_POINT_FEATURES_INITIALIZER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_ABSTRACT_FLOATING_POINT_FEATURES_INITIALIZER_HPP

#include <common/logger.hpp>
#include <cpuid.h>
#include <creg_bits.h>

namespace i386 {

class AbstractFloatingPointFeaturesInitializer {
public:
    virtual ~AbstractFloatingPointFeaturesInitializer() = default;

    void initialize_floating_point_features()
    {
        TRACE("Setting up floating point features...");
        unsigned leaf = 1, a = 0, b = 0, c = 0, d = 0;
        int result = __get_cpuid(leaf, &a, &b, &c, &d);
        if (result == 0) {
            TRACE("Unable to get CPU features from CPUID.");
        } else {
            #ifndef bit_FPU
            constexpr unsigned bit_FPU = 1 << 0;
            #endif
            if (d & bit_FPU) {
                initialize_fpu();
            }
            if (d & bit_MMX) {
                TRACE("MMX support");
            }
            if (d & bit_SSE) {
                initialize_sse();
            }
            if (d & bit_SSE2) {
                TRACE("SSE2 support");
            }
            if (c & bit_SSE3) {
                TRACE("SSE3 support");
            }
            if (c & bit_SSE4_1) {
                TRACE("SSE4.1 support");
            }
            if (c & bit_SSE4_2) {
                TRACE("SSE4.2 support");
            }
        }
    }

protected:
    virtual uintptr_t get_cr0() const = 0;
    virtual void set_cr0(uintptr_t value) const = 0;

    virtual uintptr_t get_cr4() const = 0;
    virtual void set_cr4(uintptr_t value) const = 0;

private:
    inline void fninit()
    {
        asm volatile("fninit");
    }

    void initialize_fpu()
    {
        TRACE("CPU supports an FPU. Initializing...");
        uintptr_t cr0 = get_cr0();
        cr0 = cr0 & ~(CR0_EM | CR0_TS | CR0_NE);
        set_cr0(cr0);
        fninit();
    }

    void initialize_sse()
    {
        TRACE("CPU supports SSE. Initializing...");
        uintptr_t cr0 = get_cr0();
        cr0 = cr0 & ~CR0_EM;
        cr0 = cr0 | CR0_MP;
        set_cr0(cr0);

        uintptr_t cr4 = get_cr4();
        cr4 = cr4 | CR4_OSFXSR;
        cr4 = cr4 | CR4_OSXMMEXCPT;
        set_cr4(cr4);
    }
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_ABSTRACT_FLOATING_POINT_FEATURES_INITIALIZER_HPP
