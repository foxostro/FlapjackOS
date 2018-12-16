#ifndef FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_PAGE_TABLE_HPP
#define FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_PAGE_TABLE_HPP

#include <page_size.hpp>
#include "page_table_entry.hpp"

namespace x86_64 {

struct alignas(PAGE_SIZE) PageTable {
public:
    static constexpr size_t COUNT = 512;
    PageTableEntry entries[COUNT];
};
static_assert(4096 == sizeof(PageTable),
              "The hardware Page Table object has a length of 4096 bytes.");

} // namespace x86_64

#endif // FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_PAGE_TABLE_HPP
