#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_EARLY_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_EARLY_PHYSICAL_MEMORY_MAP_HPP

#include <platform/i386/generic_early_physical_memory_map.hpp>
#include "paging_resolver.hpp"
#include <hardware_memory_management_unit.hpp>

namespace x86_64 {

class EarlyPhysicalMemoryMap : public i386::GenericEarlyPhysicalMemoryMap<PagingResolver> {
public:
    EarlyPhysicalMemoryMap(HardwareMemoryManagementUnit& mmu)
     : i386::GenericEarlyPhysicalMemoryMap<PagingResolver>(mmu)
    {}
    
    void populate_page_tables(uintptr_t begin, size_t length) override
    {
        PagingResolver& resolver = this->resolver_;
        resolver.enumerate_page_map_level_four_entries(begin, length, [&](PageMapLevelFourEntry& pml4e){
            populate_page_directory_pointer_table(pml4e);
        });
        resolver.enumerate_page_directory_pointer_table_entries(begin, length, [&](PageDirectoryPointerTableEntry& pdpte){
            populate_page_directory(pdpte);
        });
        resolver.enumerate_page_directory_entries(begin, length, [&](PageDirectoryEntry& pde){
            populate_page_table(pde);
        });
    }

private:
    void populate_page_directory_pointer_table(PageMapLevelFourEntry& pml4e)
    {
        if (pml4e.is_present() == false) {
            PageDirectoryPointerTable* pdpt = new PageDirectoryPointerTable;
            memset(pdpt, 0, sizeof(PageDirectoryPointerTable));
            uintptr_t linear_address = reinterpret_cast<uintptr_t>(pdpt);
            uintptr_t physical_address = this->mmu_.convert_logical_to_physical_address(linear_address);
            pml4e.set_address(physical_address);
            pml4e.set_present(true);
            pml4e.set_readwrite(true);
            pml4e.set_supervisor(false);
        }
    }

    void populate_page_directory(PageDirectoryPointerTableEntry& pdpte)
    {
        if (pdpte.is_present() == false) {
            PageDirectory* page_directory = new PageDirectory;
            memset(page_directory, 0, sizeof(PageDirectory));
            uintptr_t linear_address = reinterpret_cast<uintptr_t>(page_directory);
            uintptr_t physical_address = this->mmu_.convert_logical_to_physical_address(linear_address);
            pdpte.set_address(physical_address);
            pdpte.set_present(true);
            pdpte.set_readwrite(true);
            pdpte.set_supervisor(false);
        }
    }

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

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_EARLY_PHYSICAL_MEMORY_MAP_HPP