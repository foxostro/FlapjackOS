#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_HARDWARE_MEMORY_MANAGEMENT_UNIT_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_HARDWARE_MEMORY_MANAGEMENT_UNIT_HPP

#include <cstdint>
#include "creg.hpp"

namespace x86_64 {

class HardwareMemoryManagementUnit {
public:
    uint64_t get_cr3()
    {
        return x86_64::get_cr3();
    }

    void reload()
    {
        x86_64::set_cr3(x86_64::get_cr3());
    }
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_HARDWARE_MEMORY_MANAGEMENT_UNIT_HPP