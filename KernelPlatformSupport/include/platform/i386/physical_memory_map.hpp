#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PHYSICAL_MEMORY_MAP_HPP

#include <cstdint>
#include "paging_resolver.hpp"

namespace i386 {

// Provides a platform-agnostic interface to the hardware MMU memory map.
// It's physical because it's hardware.
class PhysicalMemoryMap {
public:
    static constexpr unsigned PRESENT = (1 << 0);
    static constexpr unsigned WRITABLE = (1 << 1);
    static constexpr unsigned GLOBAL = (1 << 2);
    using ProtectionFlags = unsigned;

    PhysicalMemoryMap();

    // Point the physical memory map at the paging structures active on the MMU.
    // So, whatever the MMU is using right now is what PhysicalMemoryMap will
    // now act upon.
    template<typename MMU>
    void load(MMU &mmu)
    {
        resolver_.set_cr3(mmu.get_cr3());
    }

    // Map the specified physical page to the virtual page.
    // Use `flags' to control the permissions.
    void map_page(uintptr_t physical_address,
                  uintptr_t linear_address,
                  ProtectionFlags flags);

    // Mark the specified range of virtual pages as read-only.
    void set_readonly(uintptr_t begin, uintptr_t end);

    // Invalidates the hardware MMU cache for the specified virtual addresses.
    void invalidate_pages(uintptr_t begin, uintptr_t end);

    // Invalidates the hardware MMU cache for the specified virtual address.
    void invalidate_page(uintptr_t virtual_address);

private:
    PagingResolver resolver_;
};

} // namespace i386

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PHYSICAL_MEMORY_MAP_HPP