#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER64_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER64_HPP

#include <common/data.hpp>
#include <common/elf64_parser.hpp>
#include <kernel_policy.hpp>
#include <page_frame_allocator.hpp>
#include <elf_loader.hpp>

// Loads the ELF executable into the current address space.
class ElfLoader64 : public ElfLoaderBase<elf::Parser64, elf::Elf64_Phdr> {
public:
    // Constructor.
    // physical_memory_map -- The ELF loader needs to be able adjust virtual
    //                        memory mapping to place the executable's memory
    //                        segments appropriately.
    // page_frame_allocator -- It's also necessary to be able to allocate more
    //                         physical memory to accomodate the executable's
    //                         memory segments. 
    // elf_image -- The ELF image in memory.
    ElfLoader64(PhysicalMemoryMap& physical_memory_map,
                PageFrameAllocator& page_frame_allocator,
                const Data& elf_image)
     : ElfLoaderBase(physical_memory_map, page_frame_allocator, elf_image)
    {}

    // Returns true if the image is acceptable and this loader can exec().
    bool can_exec() override
    {
        auto parser = create_parser();
        return parser.is_ia32e() && parser.is_executable();
    }
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER64_HPP
