#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PHYSICAL_MEMORY_MAP_HPP

#include <cstdint>
#include "page_directory.hpp"

namespace i386 {

// Provides a platform-agnostic interface to the hardware MMU memory map.
// It's physical because it's hardware.
class PhysicalMemoryMap {
public:
    static constexpr unsigned PRESENT = (1 << 0);
    static constexpr unsigned WRITABLE = (1 << 1);
    using ProtectionFlags = unsigned;

    PhysicalMemoryMap();

    // Map the specified physical page to the virtual page.
    // Use `flags' to control the permissions.
    void map_page(uintptr_t physical_address,
                  uintptr_t virtual_address,
                  ProtectionFlags flags);

    // Mark the specified range of virtual pages as read-only.
    void set_readonly(uintptr_t begin, uintptr_t end);

    // Invalidates the hardware MMU cache for the specified virtual addresses.
    void invalidate_pages(uintptr_t begin, uintptr_t end);

    // Invalidates the hardware MMU cache for the specified virtual address.
    void invalidate_page(uintptr_t virtual_address);

private:
    PageDirectory& get_page_directory();

    // Translate the protection flags to equivalent flags for the underlying
    // hardware paging objects.
    unsigned translate_protection_flags(ProtectionFlags flags);
};

} // namespace i386

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PHYSICAL_MEMORY_MAP_HPP