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
        UniquePointer<PageMapLevelFour> physical_pml4 = resolver.get_page_map_level_four();
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
        // AFOX_TODO: We probably want to set the entries to the weakest protection afforded any PML1 entry within it.
        set_entry_protection(get_pml4_entry(linear_address), flags);
        set_entry_protection(get_pml3_entry(linear_address), flags);
        set_entry_protection(get_pml2_entry(linear_address), flags);
        set_entry_protection(get_pml1_entry(linear_address), flags);

        set_page_frame_mapping(physical_address, linear_address, flags);
    }

    void set_page_frame_mapping(uintptr_t physical_address,
                                uintptr_t linear_address,
                                ProtectionFlags flags)
    {
        auto& pml1_entry = get_pml1_entry(linear_address);
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
        set_entry_protection(get_pml4_entry(linear_address), flags);
        set_entry_protection(get_pml3_entry(linear_address), flags);
        set_entry_protection(get_pml2_entry(linear_address), flags);
        set_entry_protection(get_pml1_entry(linear_address), flags);

        allocate_and_set_page_frame_mapping(linear_address, flags);
    }

    void allocate_and_set_page_frame_mapping(uintptr_t linear_address, ProtectionFlags flags)
    {
        auto& pml1_entry = get_pml1_entry(linear_address);
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
        set_entry_read_only(get_pml4_entry(linear_address));
        set_entry_read_only(get_pml3_entry(linear_address));
        set_entry_read_only(get_pml2_entry(linear_address));
        set_entry_read_only(get_pml1_entry(linear_address));
    }

    template<typename Entry>
    void set_entry_read_only(Entry& entry)
    {
        if (entry.is_present()) {
            entry.set_readwrite(false);
        }
    }

    PagingTopology::PageMapLevelFourController::Entry& get_pml4_entry(uintptr_t linear_address)
    {
        return pml4_->get_entry_by_offset(linear_address);
    }

    PagingTopology::PageMapLevelThreeController::Entry& get_pml3_entry(uintptr_t linear_address)
    {
        uintptr_t pml3_offset = get_pml3_offset(linear_address);
        return get_pml3(linear_address)->get_entry_by_offset(pml3_offset);
    }

    PagingTopology::PageMapLevelTwoController::Entry& get_pml2_entry(uintptr_t linear_address)
    {
        uintptr_t pml2_offset = get_pml2_offset(linear_address);
        return get_pml2(linear_address)->get_entry_by_offset(pml2_offset);
    }

    PagingTopology::PageMapLevelOneController::Entry& get_pml1_entry(uintptr_t linear_address)
    {
        uintptr_t pml1_offset = get_pml1_offset(linear_address);
        return get_pml1(linear_address)->get_entry_by_offset(pml1_offset);
    }

    uintptr_t get_pml4_offset(uintptr_t linear_address)
    {
        return linear_address;
    }

    uintptr_t get_pml3_offset(uintptr_t linear_address)
    {
        return get_pml4()->get_corresponding_pml3_offset(linear_address);
    }

    uintptr_t get_pml2_offset(uintptr_t linear_address)
    {
        return get_pml3(linear_address)->get_corresponding_pml2_offset(linear_address);
    }

    uintptr_t get_pml1_offset(uintptr_t linear_address)
    {
        return get_pml2(linear_address)->get_corresponding_pml1_offset(linear_address);
    }

    SharedPointer<PagingTopology::PageMapLevelFourController> get_pml4()
    {
        assert(pml4_);
        return pml4_;
    }

    SharedPointer<PagingTopology::PageMapLevelThreeController> get_pml3(uintptr_t linear_address)
    {
        return get_pml4_entry(linear_address).get_pml3();
    }

    SharedPointer<PagingTopology::PageMapLevelTwoController> get_pml2(uintptr_t linear_address)
    {
        return get_pml3(linear_address)->get_entry_by_offset(get_pml3_offset(linear_address)).get_pml2();
    }

    SharedPointer<PagingTopology::PageMapLevelOneController> get_pml1(uintptr_t linear_address)
    {
        return get_pml2(linear_address)->get_entry_by_offset(get_pml2_offset(linear_address)).get_pml1();
    }
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_MANAGED_PHYSICAL_MEMORY_MAP_HPP
