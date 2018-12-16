#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_HARDWARE_MEMORY_MANAGEMENT_UNIT_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_HARDWARE_MEMORY_MANAGEMENT_UNIT_HPP

#include <cstdint>
#include "creg.hpp"
#include "kernel_image_info.hpp" // for KERNEL_VIRTUAL_START_ADDR

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

    // Convert the given physical address to a logical address.
    // These addresses are easily converted between physical and virtual as they are
    // always offset by KERNEL_VIRTUAL_START_ADDR.
    uintptr_t convert_physical_to_logical_address(uint32_t physical_address)
    {
        return physical_address + KERNEL_VIRTUAL_START_ADDR;
    }

    // Convert the given logical address to a physical address.
    // These addresses are easily converted between physical and virtual as they are
    // always offset by KERNEL_VIRTUAL_START_ADDR.
    uint32_t convert_logical_to_physical_address(uintptr_t logical_address)
    {
        assert(logical_address >= KERNEL_VIRTUAL_START_ADDR);
        return logical_address - KERNEL_VIRTUAL_START_ADDR;
    }
};

} // namespace i386

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_HARDWARE_MEMORY_MANAGEMENT_UNIT_HPP