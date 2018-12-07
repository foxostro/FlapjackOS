#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_KERNEL_ADDRESS_SPACE_BOOTSTRAPPER_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_KERNEL_ADDRESS_SPACE_BOOTSTRAPPER_HPP

#include "physical_memory_map.hpp"
#include "mmu.hpp"
#include <page_size.hpp> // for PAGE_SIZE
#include <logical_addressing.hpp> // for KERNEL_MEMORY_REGION_SIZE

namespace i386 {

// Populate the MMU paging structures for kernel logical memory.
// Also owns the statically allocated storage for those paging structures.
//
// This object ensures the MMU paging structures are allocated in mapped memory
// such that actions to map pages in kernel logical memory do not require
// additional memory to be allocated.
//
// The hardware paging structures used to map kernel logical memory need to be
// allocated in memory themselves. If allocated in DATA or BSS, those objects
// will cause the kernel image to become larger than the 3MB limit imposed by
// the bootstrap code. So, there's a whole dance to bootstrap the address space.
class KernelAddressSpaceBootstrapper {
public:
    KernelAddressSpaceBootstrapper();

    template<typename MMU>
    void prepare_address_space(MMU &mmu)
    {
        prepare_address_space(mmu.get_cr3());
    }

    void prepare_address_space(uint64_t cr3);

    inline PhysicalMemoryMap& get_physical_memory_map()
    {
        return physical_memory_map_;
    }

    inline const PhysicalMemoryMap& get_physical_memory_map() const
    {
        return physical_memory_map_;
    }

private:
    static constexpr size_t NUMBER_OF_PAGE_TABLES = KERNEL_MEMORY_REGION_SIZE / PAGE_SIZE / PageTable::NUMBER_OF_PAGE_TABLE_ENTRIES;
    PageTable page_tables_[NUMBER_OF_PAGE_TABLES];
    size_t count_;
    PageTable* next_page_table_;
    PhysicalMemoryMap physical_memory_map_;

    void prepare_page_directory_entry(PageDirectoryEntry& pde);
    PageTable* get_next_page_table();
};

} // namespace i386

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_KERNEL_ADDRESS_SPACE_BOOTSTRAPPER_HPP