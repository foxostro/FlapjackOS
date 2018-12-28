#ifndef FLAPJACKOS_KERNEL_INCLUDE_MULTIBOOT_MODULE_ENUMERATOR_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_MULTIBOOT_MODULE_ENUMERATOR_HPP

#include <multiboot.h>
#include <panic.h>
#include <cstdint>
#include <cassert>

// Parses the mulitboot info structure and enumerates the available modules.
class MultibootModuleEnumerator {
public:
    template<typename MMU>
    MultibootModuleEnumerator(MMU& mmu, multiboot_info_t* mb_info)
     : mb_info_(mb_info),
       index_(0)
    {
        assert(mb_info_);
        uintptr_t mods_address = mmu.convert_physical_to_logical_address(mb_info_->mods_addr);
        entry_ = reinterpret_cast<multiboot_module_t*>(mods_address);
        count_ = static_cast<size_t>(mb_info_->mods_count);
    }

    bool has_next() const
    {
        return index_ < count_;
    }

    multiboot_module_t* get_next()
    {
        assert(has_next());
        multiboot_module_t* result = entry_;
        index_++;
        entry_++;
        return result;
    }

    template<typename Function>
    void enumerate(Function&& function)
    {
        while (has_next()) {
            function(get_next());
        }
    }

private:
    multiboot_info_t* mb_info_;
    multiboot_module_t* entry_;
    size_t count_;
    size_t index_;
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_MULTIBOOT_MODULE_ENUMERATOR_HPP
