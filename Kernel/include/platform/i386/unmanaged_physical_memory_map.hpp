#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_UNMANAGED_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_UNMANAGED_PHYSICAL_MEMORY_MAP_HPP

#include "generic_unmanaged_physical_memory_map.hpp"
#include "paging_resolver.hpp"
#include <hardware_memory_management_unit.hpp>
#include "logger.hpp"

namespace i386 {

class UnmanagedPhysicalMemoryMap : public GenericUnmanagedPhysicalMemoryMap<PagingResolver>
{
public:
    UnmanagedPhysicalMemoryMap(HardwareMemoryManagementUnit& mmu)
     : GenericUnmanagedPhysicalMemoryMap<PagingResolver>(mmu)
    {}
    
    void populate_page_tables(uintptr_t begin, size_t length) override
    {
        PagingResolver& resolver = this->resolver_;
        resolver.enumerate_page_directory_entries(begin, length, [&](PageDirectoryEntry& pde){
            populate_page_table(pde);
        });
    }

private:
    void populate_page_table(PageDirectoryEntry& pde)
    {
        if (pde.is_present() == false) {
            PageTable* page_table = new PageTable;
            memset(page_table, 0, sizeof(PageTable));
            uintptr_t linear_address = reinterpret_cast<uintptr_t>(page_table);
            uintptr_t physical_address = this->mmu_.convert_logical_to_physical_address(linear_address);
            pde.set_address(physical_address);
            pde.set_present(true);
            pde.set_readwrite(true);
            pde.set_supervisor(false);
        }
    }
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_UNMANAGED_PHYSICAL_MEMORY_MAP_HPP