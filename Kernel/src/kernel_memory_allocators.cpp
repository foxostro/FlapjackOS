#include "kernel_memory_allocators.hpp"
#include <logger.hpp>
#include <common/global_allocator.hpp>
#include <malloc/malloc_zone.hpp>
#include <kernel_address_space_bootstrap_operation.hpp>
#include <cleanup_kernel_memory_map_operation.hpp>
#include <page_frame_allocator_configuration_operation.hpp>



// Parses the mulitboot memory map and enumerates the available page frames.
class MemoryMapPageFrameEnumerator {
public:
    MemoryMapPageFrameEnumerator(multiboot_info_t *mb_info)
     : mb_info_(mb_info),
       entry_(nullptr),
       limit_(nullptr),
       curr_page_frame_(0),
       remaining_length_of_entry_(0)
    {
        assert(mb_info_);

        if (!(mb_info_->flags & MULTIBOOT_MEMORY_INFO)) {
            panic("The bootloader did not provide memory information.");
        }

        entry_ = (multiboot_memory_map_t *)convert_physical_to_logical_address(mb_info_->mmap_addr);
        limit_ = entry_ + mb_info_->mmap_length;

        assert(entry_->size > 0);
        assert(entry_->type == MULTIBOOT_MEMORY_AVAILABLE);

        remaining_length_of_entry_ = entry_->length_low;
        curr_page_frame_ = entry_->base_addr_low;
    }

    MemoryMapPageFrameEnumerator(const MemoryMapPageFrameEnumerator &enumerator) = default;

    bool has_next() const
    {
        if (remaining_length_of_entry_ > 0) {
            return true;
        } else {
            return nullptr != get_next_valid_entry(entry_);            
        }
    }

    uintptr_t get_next()
    {
        assert(has_next());

        if (remaining_length_of_entry_ == 0) {
            scan_to_next_valid_entry();
        }

        uintptr_t result = curr_page_frame_;

        remaining_length_of_entry_ -= PAGE_SIZE;
        curr_page_frame_ += PAGE_SIZE;

        return result;
    }

private:
    multiboot_info_t *mb_info_;
    multiboot_memory_map_t *entry_;
    multiboot_memory_map_t *limit_;
    uintptr_t curr_page_frame_;
    size_t remaining_length_of_entry_;

    void scan_to_next_valid_entry()
    {
        TRACE("scanning to next valid entry...");
        entry_ = get_next_valid_entry(entry_);
        assert(entry_ != nullptr);
        remaining_length_of_entry_ = entry_->length_low;
        curr_page_frame_ = entry_->base_addr_low;
        TRACE("found entry with size of %u", (unsigned)remaining_length_of_entry_);
    }

    multiboot_memory_map_t* get_next_valid_entry(multiboot_memory_map_t *entry) const
    {
        while (entry < limit_ && entry->size > 0) {
            entry = (multiboot_memory_map_t*)((uintptr_t)entry + entry->size + sizeof(entry->size));
            if (entry->type == MULTIBOOT_MEMORY_AVAILABLE) {
                return entry;
            }
        }
        return nullptr;
    }
};



KernelMemoryAllocators*
KernelMemoryAllocators::create(multiboot_info_t *mb_info,
                               TextTerminal &terminal)
{
    using storage_type = typename std::aligned_storage<sizeof(KernelMemoryAllocators), alignof(KernelMemoryAllocators)>::type;
    static storage_type storage;
    return new (&storage) KernelMemoryAllocators(mb_info, terminal);
}

KernelMemoryAllocators::KernelMemoryAllocators(multiboot_info_t *mb_info,
                                               TextTerminal &terminal)
 : mb_info_(mb_info),
   terminal_(terminal)
{
    report_installed_memory();
    initialize_kernel_break_allocator();
    prepare_kernel_memory_map();
    initialize_page_frame_allocator();
    initialize_contiguous_memory_allocator();
    initialize_kernel_malloc();
    report_free_page_frames();
}

void KernelMemoryAllocators::report_installed_memory()
{
    TRACE("%u KB low memory, %u MB high memory",
          mb_info_->mem_lower, mb_info_->mem_upper/1024);
    terminal_.printf("%u KB low memory, %u MB high memory\n",
                     mb_info_->mem_lower, mb_info_->mem_upper/1024);
}

void KernelMemoryAllocators::report_free_page_frames()
{
    TRACE("Number of free page frames is %u (%uMB)",
          (unsigned)page_frame_allocator_.get_number_of_free_page_frames(),
          (unsigned)page_frame_allocator_.get_number_of_free_page_frames()*4/1024);
    terminal_.printf("Number of free page frames is %u (%uMB)\n",
                     (unsigned)page_frame_allocator_.get_number_of_free_page_frames(),
                     (unsigned)page_frame_allocator_.get_number_of_free_page_frames()*4/1024);
}

void KernelMemoryAllocators::initialize_kernel_break_allocator()
{
    kernel_break_allocator_.reset((void*)g_kernel_image_end);
}

void KernelMemoryAllocators::initialize_page_frame_allocator()
{
    terminal_.printf("Page frame allocator will use %u bytes (%u KB)\n",
                     (unsigned)sizeof(PageFrameAllocator),
                     (unsigned)sizeof(PageFrameAllocator)/1024);

    uintptr_t break_address = (uintptr_t)kernel_break_allocator_.get_kernel_break();
    using Op = PageFrameAllocatorConfigurationOperation<MemoryMapPageFrameEnumerator>;
    Op operation(break_address,
                 MemoryMapPageFrameEnumerator(mb_info_));
    operation.configure(page_frame_allocator_);
}

void KernelMemoryAllocators::prepare_kernel_memory_map()
{
    {
        CleanupKernelMemoryMapOperation operation;
        operation.cleanup_kernel_memory_map();
    }

    {
        KernelAddressSpaceBootstrapOperation operation(kernel_break_allocator_);
        operation.prepare_address_space();
    }
}

void KernelMemoryAllocators::initialize_contiguous_memory_allocator()
{
    void *memory = kernel_break_allocator_.malloc(sizeof(KernelContiguousMemoryAllocator));
    contiguous_memory_allocator_ = new (memory) KernelContiguousMemoryAllocator(kernel_break_allocator_, page_frame_allocator_);
}

void KernelMemoryAllocators::initialize_kernel_malloc()
{
    constexpr size_t EIGHT_MB = 8 * 1024 * 1024;
    size_t length = EIGHT_MB;
    assert(length > 0);
    TRACE("Malloc zone size is %u KB.", (unsigned)length/1024);
    uintptr_t begin = (uintptr_t)contiguous_memory_allocator_->malloc(length);

    constexpr int MAGIC_NUMBER_UNINITIALIZED_HEAP = 0xCD;
    TRACE("Clearing the malloc zone to 0x%x.", MAGIC_NUMBER_UNINITIALIZED_HEAP);
    memset((void*)begin, MAGIC_NUMBER_UNINITIALIZED_HEAP, length);
    TRACE("Finished clearing the malloc zone.");

    MemoryAllocator *alloc = MallocZone::create(begin, length);
    set_global_allocator(alloc);
}
