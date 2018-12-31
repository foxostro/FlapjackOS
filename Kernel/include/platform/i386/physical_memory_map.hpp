#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PHYSICAL_MEMORY_MAP_HPP

#include "generic_physical_memory_map.hpp"
#include "paging_resolver.hpp"
#include "hardware_memory_management_unit.hpp"
#include "logger.hpp"

namespace i386 {

template<typename MemoryManagementUnit>
class PhysicalMemoryMap : public GenericPhysicalMemoryMap<i386::PagingResolver<MemoryManagementUnit>>
{
public:
    PhysicalMemoryMap(MemoryManagementUnit& mmu)
     : GenericPhysicalMemoryMap<i386::PagingResolver<MemoryManagementUnit>>(mmu)
    {}
    
    void populate_page_tables(uintptr_t begin, size_t length) override
    {
        TRACE("begin");
        using PagingResolver = i386::PagingResolver<MemoryManagementUnit>;
        PagingResolver& resolver = this->resolver_;
        i386::PageDirectory* pd = resolver.get_page_directory();
        assert(pd);
        resolver.enumerate_page_directory_entries(*pd, begin, length, [&](PageDirectoryEntry& pde){
            populate_page_table(pde);
        });
        TRACE("end");
    }

private:
    void populate_page_table(PageDirectoryEntry& pde)
    {
        if (pde.is_present() == false) {
            TRACE("begin");
            PageTable* page_table = new PageTable;
            memset(page_table, 0, sizeof(PageTable));
            TRACE("page_table=%p", page_table);
            uintptr_t linear_address = reinterpret_cast<uintptr_t>(page_table);
            uintptr_t physical_address = this->mmu_.convert_logical_to_physical_address(linear_address);
            pde.set_address(physical_address);
            pde.set_present(true);
            pde.set_readwrite(true);
            pde.set_supervisor(linear_address >= KERNEL_VIRTUAL_START_ADDR);
            TRACE("end");
        }
    }
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PHYSICAL_MEMORY_MAP_HPP