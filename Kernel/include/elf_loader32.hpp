#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER32_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER32_HPP

#include <common/data.hpp>
#include <common/elf32_parser.hpp>
#include <kernel_policy.hpp>
#include <page_frame_allocator.hpp>
#include <elf_loader.hpp>

// Loads the ELF executable into the current address space.
class ElfLoader32 : public ElfLoaderBase<Elf::Parser32, Elf::Elf32_Phdr> {
public:
    using ElfParser = Elf::Parser32;

    // Constructor.
    // physical_memory_map -- The ELF loader needs to be able adjust virtual
    //                        memory mapping to place the executable's memory
    //                        segments appropriately.
    // elf_image -- The ELF image in memory.
    ElfLoader32(PhysicalMemoryMap& physical_memory_map,
                const Data& elf_image)
     : ElfLoaderBase(physical_memory_map, elf_image)
    {}

    // Returns true if the image is acceptable and this loader can exec().
    bool can_exec() override
    {
        auto parser = create_parser();
        return parser.is_ia32() && parser.is_executable();
    }
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER32_HPP
