#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PAGE_DIRECTORY_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PAGE_DIRECTORY_HPP

#include <page_size.hpp>
#include "page_directory_entry.hpp"

namespace i386 {

struct alignas(PAGE_SIZE) PageDirectory {
public:
    static constexpr size_t COUNT = 1024;
    PageDirectoryEntry entries[COUNT];
};
static_assert(4096 == sizeof(PageDirectory),
              "The hardware Page Directory object has a length of 4096 bytes.");

} // namespace i386

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PAGE_DIRECTORY_HPP