#ifndef FLAPJACKOS_KERNEL_INCLUDE_LOGICAL_ADDRESSING_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_LOGICAL_ADDRESSING_HPP

#include "kernel_image_info.hpp"
#include <cassert>

// The kernel memory region is fixed at 1GB.
constexpr size_t KERNEL_MEMORY_REGION_SIZE = 1024*1024*1024;

// Convert the given physical address to a logical address.
// These addresses are easily converted between physical and virtual as they are
// always offset by KERNEL_VIRTUAL_START_ADDR.
inline uintptr_t convert_physical_to_logical_address(uintptr_t physical_address)
{
    return physical_address + KERNEL_VIRTUAL_START_ADDR;
}

// Convert the given logical address to a physical address.
// These addresses are easily converted between physical and virtual as they are
// always offset by KERNEL_VIRTUAL_START_ADDR.
inline uintptr_t convert_logical_to_physical_address(uintptr_t logical_address)
{
    assert(logical_address >= KERNEL_VIRTUAL_START_ADDR);
    return logical_address - KERNEL_VIRTUAL_START_ADDR;
}

#endif // FLAPJACKOS_KERNEL_INCLUDE_LOGICAL_ADDRESSING_HPP
