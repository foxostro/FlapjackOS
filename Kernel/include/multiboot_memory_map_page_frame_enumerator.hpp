#ifndef FLAPJACKOS_KERNEL_INCLUDE_MULTIBOOT_MEMORY_MAP_PAGE_FRAME_ENUMERATOR_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_MULTIBOOT_MEMORY_MAP_PAGE_FRAME_ENUMERATOR_HPP

#include <multiboot.h>
#include <panic.h>
#include <cstdint>
#include <cassert>

// Parses the mulitboot memory map and enumerates the available page frames.
class MultibootMemoryMapPageFrameEnumerator {
public:
    template<typename MMU>
    MultibootMemoryMapPageFrameEnumerator(MMU& mmu, multiboot_info_t* mb_info)
     : mb_info_(mb_info)
    {
        assert(mb_info_);

        if (!(mb_info_->flags & MULTIBOOT_MEMORY_INFO)) {
            panic("The bootloader did not provide memory information.");
        }

        entry_ = (multiboot_memory_map_t *)mmu.convert_physical_to_logical_address(mb_info_->mmap_addr);
        limit_ = entry_ + mb_info_->mmap_length;

        assert(entry_->size > 0);
        assert(entry_->type == MULTIBOOT_MEMORY_AVAILABLE);

        remaining_length_of_entry_ = entry_->length_low;
        curr_page_frame_ = entry_->base_addr_low;
    }

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
    multiboot_info_t* mb_info_;
    multiboot_memory_map_t* entry_;
    multiboot_memory_map_t* limit_;
    uintptr_t curr_page_frame_;
    size_t remaining_length_of_entry_;

    void scan_to_next_valid_entry()
    {
        entry_ = get_next_valid_entry(entry_);
        assert(entry_ != nullptr);
        remaining_length_of_entry_ = entry_->length_low;
        curr_page_frame_ = entry_->base_addr_low;
    }

    multiboot_memory_map_t* get_next_valid_entry(multiboot_memory_map_t* entry) const
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

#endif // FLAPJACKOS_KERNEL_INCLUDE_MULTIBOOT_MEMORY_MAP_PAGE_FRAME_ENUMERATOR_HPP
