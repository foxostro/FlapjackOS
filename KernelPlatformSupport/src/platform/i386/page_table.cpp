#include <platform/i386/page_table.hpp>

namespace i386 {

size_t PageTable::index_for_virtual_address(uintptr_t virtual_address) const
{
    constexpr uint32_t PTE_INDEX_SHIFT = 12;
    constexpr uint32_t PTE_INDEX_MASK = 0x03FF;
    return (uint32_t)virtual_address >> PTE_INDEX_SHIFT & PTE_INDEX_MASK;
}

} // namespace i386