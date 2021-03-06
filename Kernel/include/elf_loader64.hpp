#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER64_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER64_HPP

#include <common/data.hpp>
#include <common/elf64_parser.hpp>
#include <kernel_policy.hpp>
#include <elf_loader.hpp>

// Loads the ELF executable into the current address space.
class ElfLoader64 : public ElfLoaderBase<Elf::Parser64, Elf::Elf64_Phdr> {
public:
    // Constructor.
    // physical_memory_map -- The ELF loader needs to be able adjust virtual
    //                        memory mapping to place the executable's memory
    //                        segments appropriately.
    // elf_image -- The ELF image in memory.
    ElfLoader64(PhysicalMemoryMap& physical_memory_map,
                const Data& elf_image)
     : ElfLoaderBase(physical_memory_map, elf_image)
    {}

    // Returns true if the image is acceptable and this loader can exec().
    bool can_exec() override
    {
        auto parser = create_parser();
        return parser.is_ia32e() && parser.is_executable();
    }
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER64_HPP
