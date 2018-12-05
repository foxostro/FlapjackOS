#include <platform/i386/mmu.hpp>
#include <platform/i386/creg.hpp>
#include <logical_addressing.hpp>

namespace i386 {

uintptr_t get_page_for_address(uintptr_t address)
{
    return address & ~(PAGE_SIZE-1);
}

void invlpg(void* virtual_address)
{
    asm volatile("invlpg (%0)" : : "b"(virtual_address) : "memory");
}

PageDirectory& get_current_page_directory()
{
    return *(PageDirectory*)convert_physical_to_logical_address(get_cr3());
}

const PageDirectory& get_current_page_directory_const()
{
    return *(const PageDirectory*)convert_physical_to_logical_address(get_cr3());
}

} // namespace i386