#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_EXTRACT_PAGE_MAP_OPERATION_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_EXTRACT_PAGE_MAP_OPERATION_HPP

#include <platform/i386/page_frame_controller.hpp>
#include <platform/i386/page_map_level_one_controller.hpp>
#include <platform/i386/page_map_level_two_controller.hpp>
#include <platform/i386/page_table.hpp>
#include <platform/i386/page_directory.hpp>
#include <page_frame_allocator.hpp>
#include <hardware_memory_management_unit.hpp>
#include <logger.hpp>

namespace i386 {

class ExtractPageMapOperation {
public:
    using PML1Ptr = SharedPointer<PagingTopology::PageMapLevelOneController>;
    using PML2Ptr = SharedPointer<PagingTopology::PageMapLevelTwoController>;

    ExtractPageMapOperation(HardwareMemoryManagementUnit& mmu)
     : mmu_(mmu)
    {}

    PML2Ptr extract()
    {
        return make_pml2();
    }

private:
    HardwareMemoryManagementUnit& mmu_;

    PML2Ptr make_pml2()
    {
        TRACE("begin");
        PageDirectory* page_directory = get_current_page_directory();
        assert(page_directory);
        PML2Ptr pml2{new PageMapLevelTwoController{page_directory}};
        pml2->set_should_leak();
        configure_pml2(*pml2);
        TRACE("end");
        return pml2;
    }

    PageDirectory* get_current_page_directory()
    {
        return reinterpret_cast<PageDirectory*>(mmu_.convert_physical_to_logical_address(mmu_.get_cr3()));
    }

    void configure_pml2(PagingTopology::PageMapLevelTwoController& pml2)
    {
        TRACE("begin");
        PageDirectory* page_directory = get_current_page_directory();
        assert(page_directory);
        for (size_t i = 0, n = pml2.get_number_of_entries(); i < n; ++i) {
            auto& pde = page_directory->entries[i];
            auto& pml2_entry = pml2.get_entry(i);
            TRACE("configure_pml2_entry, i=%d", (int)i);
            configure_pml2_entry(pml2_entry, pde);
        }
        TRACE("end");
    }

    void configure_pml2_entry(PagingTopology::PageMapLevelTwoController::Entry& pml2_entry, PageDirectoryEntry& pde)
    {
        TRACE("begin");
        pml2_entry.set_pml1(make_pml1(get_page_table(pde)));
        TRACE("end");
    }

    PageTable* get_page_table(PageDirectoryEntry& pde)
    {
        if (pde.is_present()) {
            uintptr_t physical = pde.get_address();
            assert(physical != 0);
            return reinterpret_cast<PageTable*>(mmu_.convert_physical_to_logical_address(physical));
        } else {
            return nullptr;
        }
    }

    PML1Ptr make_pml1(PageTable* page_table)
    {
        TRACE("begin, page_table=%p", page_table);
        TRACE("mmu_=%p", &mmu_);
        i386::PageMapLevelOneController* ptr = new i386::PageMapLevelOneController{mmu_, page_table};
        TRACE("0");
        PML1Ptr pml1{ptr};
        TRACE("a");
        pml1->set_should_leak();
        TRACE("b");
        if (page_table) {
            TRACE("c");
            configure_pml1(*pml1, page_table);
        }
        TRACE("end");
        return pml1;
    }

    void configure_pml1(PagingTopology::PageMapLevelOneController &pml1, PageTable* page_table)
    {
        TRACE("begin, page_table=%p", page_table);
        assert(page_table);
        for (size_t i = 0, n = pml1.get_number_of_entries(); i < n; ++i) {
            TRACE("i = %d", (int)i);
            auto& pte = page_table->entries[i];
            TRACE("get_entry");
            auto& pml1_entry = pml1.get_entry(i);
            TRACE("get_address");
            uintptr_t address = static_cast<uintptr_t>(pte.get_address());
            if (address != 0) {
                TRACE("set_page_frame");
                pml1_entry.set_page_frame(new PageFrameController{address});
            }
        }
        TRACE("end");
    }
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_EXTRACT_PAGE_MAP_OPERATION_HPP
