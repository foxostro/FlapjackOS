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
        (void)physical_address;
        (void)linear_address;
        (void)flags;
        assert(!"stub");
    }

    void set_readonly(uintptr_t begin, uintptr_t end) override
    {
        (void)begin;
        (void)end;
        assert(!"stub");
    }

    void invalidate_pages(uintptr_t begin, uintptr_t end) override
    {
        for (uintptr_t page = begin; page < end; page += PAGE_SIZE) {
            invalidate_page(page);
        }
    }

    void invalidate_page(uintptr_t linear_address) override
    {
#ifdef TESTING
        (void)linear_address;
#else
        asm volatile("invlpg (%0)" : : "b"(linear_address) : "memory");
#endif
    }

private:
    HardwareMemoryManagementUnit& mmu_;
    SharedPointer<PagingTopology::PageMapLevelTwoController> pml2_;
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_CONCRETE_PHYSICAL_MEMORY_MAP_HPP
