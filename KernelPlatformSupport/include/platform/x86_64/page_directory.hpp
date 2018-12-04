#ifndef FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_PAGE_DIRECTORY_HPP
#define FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_PAGE_DIRECTORY_HPP

#include <page_size.hpp>
#include "page_directory_entry.hpp"

namespace x86_64 {

struct alignas(PAGE_SIZE) PageDirectory {
public:
    static constexpr size_t COUNT = 512;
    PageDirectoryEntry entries[COUNT];
};
static_assert(4096 == sizeof(PageDirectory),
              "The hardware Page Directory object has a length of 4096 bytes.");

} // namespace x86_64

#endif // FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_PAGE_DIRECTORY_HPP
