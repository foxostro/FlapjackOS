#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_FLOATING_POINT_FEATURES_INITIALIZER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_FLOATING_POINT_FEATURES_INITIALIZER_HPP

#include <platform/i386/abstract_floating_point_features_initializer.hpp>
#include <platform/x86_64/creg.hpp>

namespace x86_64 {

class FloatingPointFeaturesInitializer final : public i386::AbstractFloatingPointFeaturesInitializer {
protected:
    uintptr_t get_cr0() const override
    {
        return x86_64::get_cr0();
    }

    void set_cr0(uintptr_t value) const override
    {
        return x86_64::set_cr0(value);
    }

    uintptr_t get_cr4() const override
    {
        return x86_64::get_cr4();
    }

    void set_cr4(uintptr_t value) const override
    {
        return x86_64::set_cr4(value);
    }
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_FLOATING_POINT_FEATURES_INITIALIZER_HPP
