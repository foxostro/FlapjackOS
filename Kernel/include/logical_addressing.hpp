#pragma once

#include "kernel_image_info.hpp"

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
