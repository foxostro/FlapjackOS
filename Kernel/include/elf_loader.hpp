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
    // page_frame_allocator -- It's also necessary to be able to allocate more
    //                         physical memory to accomodate the executable's
    //                         memory segments. 
    // elf_image -- The ELF image in memory.
    ElfLoaderBase(PhysicalMemoryMap& physical_memory_map,
                  PageFrameAllocator& page_frame_allocator,
                  const Data& elf_image)
     : physical_memory_map_(physical_memory_map),
       page_frame_allocator_(page_frame_allocator),
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
    PageFrameAllocator& page_frame_allocator_;
    const Data& image_;

    void process_program_header(const ProgramHeader& header)
    {
        if (Elf::PT_LOAD == header.p_type) {
            action_load(header);
        }
    }

    void action_load(const ProgramHeader& header)
    {
        // AFOX_TODO: need some way to track ownership of page frames and free them when they are no longer in use
        uintptr_t physical_address = page_frame_allocator_.allocate(header.p_memsz);
        uintptr_t linear_address = header.p_vaddr;

        // Map the page so the kernel can populate its contents.
        // AFOX_TODO: We probably want action_load() to map a range of pages, actually.
        physical_memory_map_.map_page(physical_address, linear_address, WRITABLE | SUPERVISOR);
        
        // Populate the program segment. The segment in memory may be larger than
        // the data region in the file. The remainder is cleared to zero.
        memset(reinterpret_cast<char*>(linear_address), 0, header.p_memsz);
        memcpy(reinterpret_cast<char*>(linear_address),
               image_.bytes + header.p_offset,
               header.p_filesz);
        
        // Map again with the appropriate protection. This may make it read-only.
        physical_memory_map_.map_page(physical_address, linear_address, get_protection_flags(header));
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
