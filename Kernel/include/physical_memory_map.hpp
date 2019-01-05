#ifndef FLAPJACKOS_KERNEL_INCLUDE_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PHYSICAL_MEMORY_MAP_HPP

#include <cstdint>
#include <cstddef>

// Interface for the physical memory map.
// The physical memory map is a memory map for use in the harware MMU.
// There are some legitimate reasons why the virtual and physical memory maps
// might differ, such as Demand-paging and Copy-on-write.
class PhysicalMemoryMap
{
public:
    using ProtectionFlags = unsigned;

    static constexpr unsigned PRESENT = (1 << 0);
    static constexpr unsigned WRITABLE = (1 << 1);
    static constexpr unsigned GLOBAL = (1 << 2);
    static constexpr unsigned SUPERVISOR = (1 << 3);

    virtual ~PhysicalMemoryMap() = default;

    // Point the physical memory map at the paging structures active on the MMU.
    // So, whatever the MMU is using right now is what PhysicalMemoryMap
    // will now act upon.
    virtual void reload() = 0;

    // Allocate memory for page tables and insert them into the page directory.
    // AFOX_TODO: Remove the populate_page_tables() method.
    virtual void populate_page_tables(uintptr_t begin, size_t length) = 0;

    // Map the specified physical page to the virtual page.
    // Use `flags' to control the permissions.
    virtual void map_page(uintptr_t physical_address,
                          uintptr_t linear_address,
                          ProtectionFlags flags) = 0;

    // Mark the specified range of virtual pages as read-only.
    virtual void set_readonly(uintptr_t begin, uintptr_t end) = 0;
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_PHYSICAL_MEMORY_MAP_HPP