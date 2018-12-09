#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PAGE_TABLE_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PAGE_TABLE_HPP

#include <page_size.hpp>
#include "page_table_entry.hpp"

namespace i386 {

struct alignas(PAGE_SIZE) PageTable {
public:
    static constexpr size_t COUNT = 1024;
    PageTableEntry entries[COUNT];
};
static_assert(4096 == sizeof(PageTable),
              "The hardware Page Table object has a length of 4096 bytes.");

} // namespace i386

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PAGE_TABLE_HPP