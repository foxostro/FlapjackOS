#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_FLOATING_POINT_FEATURES_INITIALIZER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_FLOATING_POINT_FEATURES_INITIALIZER_HPP

#include <platform/i386/abstract_floating_point_features_initializer.hpp>
#include <platform/i386/creg.hpp>

namespace i386 {

class FloatingPointFeaturesInitializer final : public AbstractFloatingPointFeaturesInitializer {
protected:
    uintptr_t get_cr0() const override
    {
        return i386::get_cr0();
    }

    void set_cr0(uintptr_t value) const override
    {
        return i386::set_cr0(value);
    }

    uintptr_t get_cr4() const override
    {
        return i386::get_cr4();
    }

    void set_cr4(uintptr_t value) const override
    {
        return i386::set_cr4(value);
    }
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_FLOATING_POINT_FEATURES_INITIALIZER_HPP
