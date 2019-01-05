#ifndef FLAPJACKOS_KERNEL_INCLUDE_HARDWARE_MEMORY_MANAGEMENT_UNIT_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_HARDWARE_MEMORY_MANAGEMENT_UNIT_HPP

#include <cstdint>

// Interface for the hardware MMU.
// Includes logic for converting between kernel logical addresses and physical
// addresses because this is heavily dependent on the details of how the kernel
// was mapped into the higher-half.
// AFOX_TODO: This seems like a SRP violation. The class should be broken up.
class HardwareMemoryManagementUnit {
public:
    // Gets the
    // AFOX_TODO: Only x86 and x86_64 have CR3. So what how do I fit ARM into this API?
    virtual uintptr_t get_cr3() const = 0;

    // Reloads the hardware MMU and flushes the TLB.
    virtual void reload() = 0;

    // Invalidates the hardware MMU cache for the specified virtual addresses.
    virtual void invalidate_pages(uintptr_t begin, uintptr_t end) = 0;

    // Invalidates the hardware MMU cache for the specified virtual address.
    virtual void invalidate_page(uintptr_t linear_address) = 0;

    // Convert the given physical address to a logical address.
    // These addresses are easily converted between physical and virtual as they are
    // always offset by KERNEL_VIRTUAL_START_ADDR.
    virtual uintptr_t convert_physical_to_logical_address(uintptr_t physical_address) const = 0;

    // Convert the given logical address to a physical address.
    // These addresses are easily converted between physical and virtual as they are
    // always offset by KERNEL_VIRTUAL_START_ADDR.
    virtual uintptr_t convert_logical_to_physical_address(uintptr_t logical_address) const = 0;

    // Gets the address of the start of the kernel virtual memory region.
    // This is the start of the so-called higher-half.
    virtual uintptr_t get_kernel_virtual_start_address() const = 0;
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_HARDWARE_MEMORY_MANAGEMENT_UNIT_HPP