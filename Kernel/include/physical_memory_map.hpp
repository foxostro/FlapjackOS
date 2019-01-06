#ifndef FLAPJACKOS_KERNEL_INCLUDE_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PHYSICAL_MEMORY_MAP_HPP

#include <cstdint>
#include <cstddef>
#include <protection_flags.hpp>

// Interface for the physical memory map.
// The physical memory map is a memory map for use in the harware MMU.
// There are some legitimate reasons why the virtual and physical memory maps
// might differ, such as Demand-paging and Copy-on-write.
class PhysicalMemoryMap
{
public:
    virtual ~PhysicalMemoryMap() = default;

    // Map virtual memory with the given protection.
    // Physical page frames will be allocated for any holes in the memory map
    // in the specified region.
    virtual void map_pages(uintptr_t begin,
                           uintptr_t end,
                           ProtectionFlags flags) = 0;

    // Map the specified physical page to the virtual page.
    // Use `flags' to control the permissions.
    virtual void map_page(uintptr_t physical_address,
                          uintptr_t linear_address,
                          ProtectionFlags flags) = 0;

    // Mark the specified range of virtual pages as read-only.
    virtual void set_readonly(uintptr_t begin, uintptr_t end) = 0;
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_PHYSICAL_MEMORY_MAP_HPP