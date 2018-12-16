#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_HARDWARE_MEMORY_MANAGEMENT_UNIT_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_HARDWARE_MEMORY_MANAGEMENT_UNIT_HPP

#include <cstdint>
#include "creg.hpp"
#include "kernel_image_info.hpp" // for KERNEL_VIRTUAL_START_ADDR

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

    // Convert the given physical address to a logical address.
    // These addresses are easily converted between physical and virtual as they are
    // always offset by KERNEL_VIRTUAL_START_ADDR.
    uintptr_t convert_physical_to_logical_address(uint64_t physical_address)
    {
        return physical_address + KERNEL_VIRTUAL_START_ADDR;
    }

    // Convert the given logical address to a physical address.
    // These addresses are easily converted between physical and virtual as they are
    // always offset by KERNEL_VIRTUAL_START_ADDR.
    uint64_t convert_logical_to_physical_address(uintptr_t logical_address)
    {
        assert(logical_address >= KERNEL_VIRTUAL_START_ADDR);
        return logical_address - KERNEL_VIRTUAL_START_ADDR;
    }

    // Gets the address of the start of the kernel virtual memory region.
    // This is the start of the so-called higher-half.
    constexpr uintptr_t get_kernel_virtual_start_address()
    {
        return KERNEL_VIRTUAL_START_ADDR;
    }
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_HARDWARE_MEMORY_MANAGEMENT_UNIT_HPP