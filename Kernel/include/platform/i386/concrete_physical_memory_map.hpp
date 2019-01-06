#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_CONCRETE_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_CONCRETE_PHYSICAL_MEMORY_MAP_HPP

#include <platform/i386/extract_page_map_operation.hpp>
#include <platform/i386/unmanaged_physical_memory_map.hpp>
#include <platform/i386/managed_physical_memory_map.hpp>
#include <common/mutex.hpp>
#include <common/lock_guard.hpp>

namespace i386 {

// As an optimization, use an unmanaged physical memory map for the kernel
// region of the address space and a managed memory map for the user portion.
// This avoids allocating control structures for page tables and page frames
// in kernel memory which will never change. The performance and memory savings
// are very substantial.
class ConcretePhysicalMemoryMap : public PhysicalMemoryMap {
public:
    ConcretePhysicalMemoryMap(HardwareMemoryManagementUnit& mmu)
     : mmu_(mmu),
       unmanaged_(mmu)
    {}

    // It's impossible to use control structures to manage memory before the
    // kernel heap is ready. Call this to signal that we can use new and delete.
    void heap_is_ready()
    {
        LockGuard lock{mutex_};
        managed_ = new ManagedPhysicalMemoryMap{mmu_, KERNEL_VIRTUAL_START_ADDR-1};
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
    UnmanagedPhysicalMemoryMap unmanaged_;
    UniquePointer<PhysicalMemoryMap> managed_;
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_CONCRETE_PHYSICAL_MEMORY_MAP_HPP
