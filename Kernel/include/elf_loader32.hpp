#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER32_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER32_HPP

#include <common/data.hpp>
#include <common/elf32_parser.hpp>
#include <kernel_policy.hpp>
#include <page_frame_allocator.hpp>
#include <elf_loader.hpp>

// Loads the ELF executable into the current address space.
class ElfLoader32 : public ElfLoader, private KernelPolicy {
public:
    using ElfParser = elf32::Parser32;

    // Constructor.
    // physical_memory_map -- The ELF loader needs to be able adjust virtual
    //                        memory mapping to place the executable's memory
    //                        segments appropriately.
    // page_frame_allocator -- It's also necessary to be able to allocate more
    //                         physical memory to accomodate the executable's
    //                         memory segments. 
    // elf_image -- The ELF image in memory.
    ElfLoader32(PhysicalMemoryMap& physical_memory_map,
                PageFrameAllocator& page_frame_allocator,
                const Data& elf_image);

    // Returns true if the image is acceptable and this loader can exec().
    bool can_exec() override;

    // Load the executable image into the current address space.
    // Then, execute at the specified start address.
    unsigned exec() override
    {
        return load()();
    }

private:
    using Function = unsigned(*)();
    PhysicalMemoryMap& physical_memory_map_;
    PageFrameAllocator& page_frame_allocator_;
    const Data& image_;

    Function load();
    ElfParser create_parser();
    void process_program_header(const elf32::Elf32_Phdr& header);
    void action_load(const elf32::Elf32_Phdr& header);
    void populate_page_tables(uintptr_t begin, size_t length);
    void populate_page_table(uintptr_t linear_address);
    Data get_segment_data(const elf32::Elf32_Phdr& header);
    PhysicalMemoryMap::ProtectionFlags get_protection_flags(const elf32::Elf32_Phdr& header);
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER32_HPP
