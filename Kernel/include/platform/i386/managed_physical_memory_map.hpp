#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_MANAGED_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_MANAGED_PHYSICAL_MEMORY_MAP_HPP

#include <platform/i386/extract_page_map_operation.hpp>
#include <physical_memory_map.hpp>
#include <common/mutex.hpp>
#include <common/lock_guard.hpp>

namespace i386 {

class ManagedPhysicalMemoryMap : public PhysicalMemoryMap {
public:
    ManagedPhysicalMemoryMap(HardwareMemoryManagementUnit& mmu,
                             uintptr_t limit = 0xFFFFFFFF)
     : mmu_(mmu)
    {
        ExtractPageMapOperation operation{mmu, limit};
        pml2_ = operation.extract();
        assert(pml2_);
    }

    void map_page(uintptr_t physical_address,
                  uintptr_t linear_address,
                  ProtectionFlags flags) override
    {
        assert(pml2_);
        
        pml2_->populate(linear_address);

        // AFOX_TODO: We probably want to set the PML2 entry to the weakest protection afforded any PML1 entry within it.
        auto& pml2_entry = pml2_->get_entry_by_offset(linear_address);
        pml2_entry.set_protection(flags);

        auto& pml1_entry = pml2_->get_pml1_entry_by_offset(linear_address);
        pml1_entry.set_mapping(physical_address, flags);
        mmu_.invalidate_page(linear_address);
    }

    void set_readonly(uintptr_t begin, uintptr_t end) override
    {
        for (uintptr_t page = begin; page < end; page += PAGE_SIZE) {
            pml2_->get_pml1_entry_by_offset(page).set_readwrite(false);
        }
        mmu_.invalidate_pages(begin, end);
    }

private:
    HardwareMemoryManagementUnit& mmu_;
    SharedPointer<PagingTopology::PageMapLevelTwoController> pml2_;
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_MANAGED_PHYSICAL_MEMORY_MAP_HPP
