#ifndef FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_PAGE_DIRECTORY_POINTER_TABLE_HPP
#define FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_PAGE_DIRECTORY_POINTER_TABLE_HPP

#include <page_size.hpp>
#include "page_directory_pointer_table_entry.hpp"

namespace x86_64 {

struct alignas(PAGE_SIZE) PageDirectoryPointerTable {
public:
    static constexpr size_t COUNT = 512;
    PageDirectoryPointerTableEntry entries[COUNT];
};
static_assert(4096 == sizeof(PageDirectoryPointerTable),
              "The hardware PDPT object has a length of 4096 bytes.");

} // namespace x86_64

#endif // FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_PAGE_DIRECTORY_POINTER_TABLE_HPP
