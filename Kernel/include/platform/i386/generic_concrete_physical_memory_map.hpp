#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_GENERIC_CONCRETE_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_GENERIC_CONCRETE_PHYSICAL_MEMORY_MAP_HPP

#include <page_frame_allocator.hpp>
#include <physical_memory_map.hpp>
#include <hardware_memory_management_unit.hpp>
#include <common/mutex.hpp>
#include <common/shared_pointer.hpp>
#include <common/lock_guard.hpp>

namespace i386 {

// As an optimization, use an unmanaged physical memory map for the kernel
// region of the address space and a managed memory map for the user portion.
// This avoids allocating control structures for page tables and page frames
// in kernel memory which will never change. The performance and memory savings
// are very substantial.
template<typename Policy>
class GenericConcretePhysicalMemoryMap : public PhysicalMemoryMap {
public:
    using UnmanagedPhysicalMemoryMap = typename Policy::UnmanagedPhysicalMemoryMap;
    using ManagedPhysicalMemoryMap = typename Policy::ManagedPhysicalMemoryMap;

    GenericConcretePhysicalMemoryMap(HardwareMemoryManagementUnit& mmu,
                                     PageFrameAllocator& page_frame_allocator)
     : mmu_(mmu),
       page_frame_allocator_(page_frame_allocator),
       unmanaged_(mmu)
    {}

    // It's impossible to use control structures to manage memory before the
    // kernel heap is ready. Call this to signal that we can use new and delete.
    void heap_is_ready()
    {
        LockGuard lock{mutex_};
        managed_ = new ManagedPhysicalMemoryMap{mmu_, page_frame_allocator_};
    }
    
    // Map virtual memory with the given protection.
    // Physical page frames will be allocated for any holes in the memory map
    // in the specified region.
    void map_pages(uintptr_t begin, uintptr_t end, ProtectionFlags flags) override
    {
        assert_is_page_aligned(begin);
        assert(begin < end);
        LockGuard lock{mutex_};
        if (end < KERNEL_VIRTUAL_START_ADDR) {
            managed_->map_pages(begin, end, flags);
        } else {
            unmanaged_.map_pages(begin, end, flags);
        }
    }

    void map_page(uintptr_t physical_address,
                  uintptr_t linear_address,
                  ProtectionFlags flags) override
    {
        LockGuard lock{mutex_};
        if (linear_address < KERNEL_VIRTUAL_START_ADDR) {
            managed_->map_page(physical_address, linear_address, flags);
        } else {
            unmanaged_.map_page(physical_address, linear_address, flags);
        }
    }

    void set_readonly(uintptr_t begin, uintptr_t end) override
    {
        LockGuard lock{mutex_};
        assert(begin < end);
        if (end < KERNEL_VIRTUAL_START_ADDR) {
            managed_->set_readonly(begin, end);
        } else {
            unmanaged_.set_readonly(begin, end);
        }
    }

private:
    Mutex mutex_;
    HardwareMemoryManagementUnit& mmu_;
    PageFrameAllocator& page_frame_allocator_;
    UnmanagedPhysicalMemoryMap unmanaged_;
    UniquePointer<PhysicalMemoryMap> managed_;
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_GENERIC_CONCRETE_PHYSICAL_MEMORY_MAP_HPP
