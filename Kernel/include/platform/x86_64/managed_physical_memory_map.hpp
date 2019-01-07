#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_MANAGED_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_MANAGED_PHYSICAL_MEMORY_MAP_HPP

#include <physical_memory_map.hpp>
#include <page_frame_allocator.hpp>
#include <platform/x86_64/page_map_level_four_controller.hpp>
#include <platform/x86_64/page_map_level_three_controller.hpp>
#include <platform/x86_64/page_map_level_two_controller.hpp>
#include <platform/x86_64/page_map_level_one_controller.hpp>
#include <platform/x86_64/paging_resolver.hpp>
#include <common/mutex.hpp>
#include <common/lock_guard.hpp>
#include <common/shared_pointer.hpp>
#include <common/unique_pointer.hpp>

namespace x86_64 {

class ManagedPhysicalMemoryMap : public PhysicalMemoryMap {
public:
    ManagedPhysicalMemoryMap(HardwareMemoryManagementUnit& mmu,
                             PageFrameAllocator& page_frame_allocator)
     : mmu_(mmu),
       page_frame_allocator_(page_frame_allocator)
    {
        PagingResolver resolver{mmu};
        UniquePointer<x86_64::PageMapLevelFour> physical_pml4 = resolver.get_page_map_level_four();
        physical_pml4.set_should_leak();
        pml4_ = UniquePointer<PagingTopology::PageMapLevelFourController>(new PageMapLevelFourController{mmu_, std::move(physical_pml4)});
    }
    
    void map_pages(uintptr_t begin, uintptr_t end, ProtectionFlags flags) override
    {
        assert_is_page_aligned(begin);
        assert(begin < end);
        for (uintptr_t page = begin; page < end; page += PAGE_SIZE) {
            map_page_internal(page, flags);
        }
        mmu_.invalidate_pages(begin, end);
    }

    void map_page(uintptr_t physical_address,
                  uintptr_t linear_address,
                  ProtectionFlags flags) override
    {
        pml4_->populate(linear_address);

        // AFOX_TODO: We probably want to set the entries to the weakest protection afforded any PML1 entry within it.
        set_entry_protection(pml4_->get_pml4_entry_by_address(linear_address), flags);
        set_entry_protection(pml4_->get_pml3_entry_by_address(linear_address), flags);
        set_entry_protection(pml4_->get_pml2_entry_by_address(linear_address), flags);
        set_entry_protection(pml4_->get_pml1_entry_by_address(linear_address), flags);

        set_page_frame_mapping(physical_address, linear_address, flags);
    }

    void set_page_frame_mapping(uintptr_t physical_address,
                                uintptr_t linear_address,
                                ProtectionFlags flags)
    {
        auto& pml1_entry = pml4_->get_pml1_entry_by_address(linear_address);
        if (pml1_entry.is_present()) {
            pml1_entry.set_protection(flags);
        } else {
            pml1_entry.set_mapping(physical_address, flags);
        }
    }

    void set_readonly(uintptr_t begin, uintptr_t end) override
    {
        for (uintptr_t page = begin; page < end; page += PAGE_SIZE) {
            set_readonly_page(page);
        }
        mmu_.invalidate_pages(begin, end);
    }

private:
    HardwareMemoryManagementUnit& mmu_;
    PageFrameAllocator& page_frame_allocator_;
    SharedPointer<PagingTopology::PageMapLevelFourController> pml4_;

    void map_page_internal(uintptr_t linear_address, ProtectionFlags flags)
    {
        pml4_->populate(linear_address);

        // AFOX_TODO: We probably want to set the entries to the weakest protection afforded any PML1 entry within it.
        set_entry_protection(pml4_->get_pml4_entry_by_address(linear_address), flags);
        set_entry_protection(pml4_->get_pml3_entry_by_address(linear_address), flags);
        set_entry_protection(pml4_->get_pml2_entry_by_address(linear_address), flags);
        set_entry_protection(pml4_->get_pml1_entry_by_address(linear_address), flags);

        allocate_and_set_page_frame_mapping(linear_address, flags);
    }

    void allocate_and_set_page_frame_mapping(uintptr_t linear_address, ProtectionFlags flags)
    {
        auto& pml1_entry = pml4_->get_pml1_entry_by_address(linear_address);
        if (pml1_entry.is_present()) {
            pml1_entry.set_protection(flags);
        } else {
            // AFOX_TODO: Need to pass the allocator to the page frame controller so the page frame can be released later.
            uintptr_t page_frame = page_frame_allocator_.allocate(PAGE_SIZE);
            pml1_entry.set_mapping(page_frame, flags);
        }
    }

    template<typename Entry>
    void set_entry_protection(Entry& entry, ProtectionFlags flags)
    {
        if (entry.is_present()) {
            entry.set_protection(flags);
        }
    }

    void set_readonly_page(uintptr_t linear_address)
    {
        set_entry_read_only(pml4_->get_pml4_entry_by_address(linear_address));
        set_entry_read_only(pml4_->get_pml3_entry_by_address(linear_address));
        set_entry_read_only(pml4_->get_pml2_entry_by_address(linear_address));
        set_entry_read_only(pml4_->get_pml1_entry_by_address(linear_address));
    }

    template<typename Entry>
    void set_entry_read_only(Entry& entry)
    {
        if (entry.is_present()) {
            entry.set_readwrite(false);
        }
    }
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_MANAGED_PHYSICAL_MEMORY_MAP_HPP
