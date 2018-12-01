#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_MMU_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_MMU_HPP

#include <cstddef>
#include <cstdint>

#include "page_size.hpp"
#include "page_directory.hpp"
#include "page_table.hpp"
#include "protection_flags.hpp"

namespace i386 {

// Gets the 4KB page associated with the given address.
uintptr_t get_page_for_address(uintptr_t address);

// Invalidate the TLB entry for the specified virtual address.
void invlpg(void* virtual_address);

// Gets the page directory currently bound to the MMU.
PageDirectory& get_current_page_directory();
const PageDirectory& get_current_page_directory_const();

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_MMU_HPP