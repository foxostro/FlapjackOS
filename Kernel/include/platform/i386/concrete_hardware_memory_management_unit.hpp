#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_CONCRETE_HARDWARE_MEMORY_MANAGEMENT_UNIT_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_CONCRETE_HARDWARE_MEMORY_MANAGEMENT_UNIT_HPP

#include <hardware_memory_management_unit.hpp>
#include "creg.hpp"
#include "kernel_image_info.hpp" // for KERNEL_VIRTUAL_START_ADDR

namespace i386 {

class ConcreteHardwareMemoryManagementUnit final : public HardwareMemoryManagementUnit {
public:
    uintptr_t get_cr3() const override
    {
        return i386::get_cr3();
    }

    void reload() override
    {
        i386::set_cr3(i386::get_cr3());
    }

    // Convert the given physical address to a logical address.
    // These addresses are easily converted between physical and virtual as they are
    // always offset by KERNEL_VIRTUAL_START_ADDR.
    uintptr_t convert_physical_to_logical_address(uintptr_t physical_address) const override
    {
        return physical_address + KERNEL_VIRTUAL_START_ADDR;
    }

    // Convert the given logical address to a physical address.
    // These addresses are easily converted between physical and virtual as they are
    // always offset by KERNEL_VIRTUAL_START_ADDR.
    uintptr_t convert_logical_to_physical_address(uintptr_t logical_address) const override
    {
        assert(logical_address >= KERNEL_VIRTUAL_START_ADDR);
        return logical_address - KERNEL_VIRTUAL_START_ADDR;
    }

    // Gets the address of the start of the kernel virtual memory region.
    // This is the start of the so-called higher-half.
    uintptr_t get_kernel_virtual_start_address() const override
    {
        return KERNEL_VIRTUAL_START_ADDR;
    }

    void invalidate_pages(uintptr_t begin, uintptr_t end) override
    {
        for (uintptr_t page = begin; page < end; page += PAGE_SIZE) {
            invalidate_page(page);
        }
    }

    void invalidate_page(uintptr_t linear_address) override
    {
#ifdef TESTING
        (void)linear_address;
#else
        asm volatile("invlpg (%0)" : : "b"(linear_address) : "memory");
#endif
    }
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_CONCRETE_HARDWARE_MEMORY_MANAGEMENT_UNIT_HPP