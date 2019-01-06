#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_CONCRETE_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_CONCRETE_PHYSICAL_MEMORY_MAP_HPP

#include <platform/i386/extract_page_map_operation.hpp>
#include <physical_memory_map.hpp>

namespace i386 {

class ConcretePhysicalMemoryMap : public PhysicalMemoryMap {
public:
    ConcretePhysicalMemoryMap(HardwareMemoryManagementUnit& mmu)
     : mmu_(mmu)
    {
        ExtractPageMapOperation operation{mmu};
        pml2_ = operation.extract();
    }
    
    void reload() override
    {
        mmu_.reload();
    }
    
    void populate_page_tables(uintptr_t, size_t) override {}

    void map_page(uintptr_t physical_address,
                  uintptr_t linear_address,
                  ProtectionFlags flags) override
    {
        size_t pml2_entry_size = pml2_->get_size_governed_by_entry();
        size_t pml2_index = linear_address / pml2_entry_size;
        auto& pml2_entry = pml2_->get_entry(pml2_index);
        set_pml2_entry_protection(pml2_entry, flags);
        auto pml1 = pml2_entry.get_pml1();
        assert(pml1);
        size_t pml1_entry_size = pml1->get_size_governed_by_entry();
        size_t pml1_index = (linear_address - (pml2_index * pml2_entry_size)) / pml1_entry_size;
        map_pml1_entry(pml1->get_entry(pml1_index), physical_address, flags);
        mmu_.invalidate_page(linear_address);
    }

    void set_pml2_entry_protection(PagingTopology::PageMapLevelTwoController::Entry& pml2_entry,
                                   ProtectionFlags flags)
    {
        pml2_entry.set_readwrite((flags & WRITABLE) != 0);
        pml2_entry.set_supervisor((flags & SUPERVISOR) != 0);
    }

    void map_pml1_entry(PagingTopology::PageMapLevelOneController::Entry& pml1_entry,
                        uintptr_t physical_address,
                        ProtectionFlags flags)
    {
        set_pml1_entry_protection(pml1_entry, flags);
        set_pml1_entry_mapping(pml1_entry, physical_address);
    }

    void set_pml1_entry_protection(PagingTopology::PageMapLevelOneController::Entry& pml1_entry,
                                   ProtectionFlags flags)
    {
        pml1_entry.set_readwrite((flags & WRITABLE) != 0);
        pml1_entry.set_supervisor((flags & SUPERVISOR) != 0);
        pml1_entry.set_global((flags & GLOBAL) != 0);
    }

    void set_pml1_entry_mapping(PagingTopology::PageMapLevelOneController::Entry& pml1_entry,
                                uintptr_t physical_address)
    {
        pml1_entry.set_page_frame(create_page_frame_controller(physical_address));
        pml1_entry.set_present(true);
    }

    UniquePointer<PagingTopology::PageFrameController> create_page_frame_controller(uintptr_t page_frame)
    {
        return new i386::PageFrameController{page_frame};
    }

    void set_readonly(uintptr_t begin, uintptr_t end) override
    {
        for (uintptr_t page = begin; page < end; page += PAGE_SIZE) {
            set_readonly(page);
        }
        mmu_.invalidate_pages(begin, end);
    }

    void set_readonly(uintptr_t linear_address)
    {
        size_t pml2_entry_size = pml2_->get_size_governed_by_entry();
        size_t pml2_index = linear_address / pml2_entry_size;
        auto& pml2_entry = pml2_->get_entry(pml2_index);
        auto pml1 = pml2_entry.get_pml1();
        if (pml1) {
            size_t pml1_entry_size = pml1->get_size_governed_by_entry();
            size_t pml1_index = (linear_address - (pml2_index * pml2_entry_size)) / pml1_entry_size;
            pml1->get_entry(pml1_index).set_readwrite(false);
        }
    }

private:
    HardwareMemoryManagementUnit& mmu_;
    SharedPointer<PagingTopology::PageMapLevelTwoController> pml2_;
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_CONCRETE_PHYSICAL_MEMORY_MAP_HPP
