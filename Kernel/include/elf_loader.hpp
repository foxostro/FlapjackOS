#ifndef FLAPJACKOS_KERNEL_INCLUDE_ELF_LOADER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_ELF_LOADER_HPP

#include <common/elf.hpp>
#include <protection_flags.hpp>

// Interface for a class which can load an ELF image into memory for exec().
class ElfLoader {
public:
    virtual ~ElfLoader() = default;
    
    // Returns true if the image is acceptable and this loader can exec().
    virtual bool can_exec() = 0;
    
    // Load the executable image into the current address space.
    // Then, execute at the specified start address.
    virtual unsigned exec() = 0;
};

// Base class for a concrete implementation of ElfLoader.
template<typename ElfParser, typename ProgramHeader>
class ElfLoaderBase : public ElfLoader {
public:
    // Constructor.
    // physical_memory_map -- The ELF loader needs to be able adjust virtual
    //                        memory mapping to place the executable's memory
    //                        segments appropriately.
    // elf_image -- The ELF image in memory.
    ElfLoaderBase(PhysicalMemoryMap& physical_memory_map,
                  const Data& elf_image)
     : physical_memory_map_(physical_memory_map),
       image_(elf_image)
    {}

    // Load the executable image into the current address space.
    // Then, execute at the specified start address.
    unsigned exec() override
    {
        return load()();
    }

protected:
    using Function = unsigned(*)();

    Function load()
    {
        assert(can_exec());
        
        // AFOX_TODO: handle error conditions in exec() too!
        auto parser = create_parser();
        parser.enumerate([&](const ProgramHeader& header){
            process_program_header(header);
        });
        return reinterpret_cast<Function>(parser.get_start_address());
    }

    ElfParser create_parser()
    {
        return ElfParser{image_.length, image_.bytes};
    }

private:
    PhysicalMemoryMap& physical_memory_map_;
    const Data& image_;

    void process_program_header(const ProgramHeader& header)
    {
        if (Elf::PT_LOAD == header.p_type) {
            action_load(header);
        }
    }

    void action_load(const ProgramHeader& header)
    {
        // Map memory so the kernel can populate its contents.
        physical_memory_map_.map_pages(header.p_vaddr,
                                       header.p_vaddr+header.p_memsz,
                                       WRITABLE | SUPERVISOR);

        // Populate the program segment. The segment in memory may be larger
        // than the data region in the file. The remainder is cleared to zero.
        memset(reinterpret_cast<char*>(header.p_vaddr), 0, header.p_memsz);
        memcpy(reinterpret_cast<char*>(header.p_vaddr),
               image_.bytes + header.p_offset,
               header.p_filesz);

        // Change protection on the pages. This may make it read-only.
        physical_memory_map_.map_pages(header.p_vaddr,
                                       header.p_vaddr+header.p_memsz,
                                       get_protection_flags(header));
    }
    
    ProtectionFlags get_protection_flags(const ProgramHeader& header)
    {
        ProtectionFlags flags = 0;
        if (header.p_flags & Elf::PF_W) {
            flags = flags | WRITABLE;
        }
        // AFOX_TODO: handle the executable flag too in get_protection_flags()
        return flags;
    }
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_ELF_LOADER_HPP
