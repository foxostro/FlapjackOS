#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_HARDWARE_MEMORY_MANAGEMENT_UNIT_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_HARDWARE_MEMORY_MANAGEMENT_UNIT_HPP

#include <cstdint>
#include "creg.hpp"

namespace i386 {

class HardwareMemoryManagementUnit {
public:
    uint32_t get_cr3()
    {
        return i386::get_cr3();
    }

    void reload()
    {
        i386::set_cr3(i386::get_cr3());
    }
};

} // namespace i386

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_HARDWARE_MEMORY_MANAGEMENT_UNIT_HPP