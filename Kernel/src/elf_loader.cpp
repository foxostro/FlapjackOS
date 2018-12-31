#include <elf_loader.hpp>

ElfLoader::ElfLoader(PhysicalMemoryMap& phys_map,
                     PageFrameAllocator& page_frame_allocator,
                     const Data& elf_image)
 : physical_memory_map_(phys_map),
   page_frame_allocator_(page_frame_allocator),
   image_(elf_image)
{}

ElfLoader::Function ElfLoader::load()
{
    // AFOX_TODO: handle error conditions in exec() too!
    auto parser = create_parser();
    assert(parser.is_ia32());
    assert(parser.is_executable());
    parser.enumerate([&](const elf32::Elf32_Phdr& header){
        process_program_header(header);
    });
    return reinterpret_cast<Function>(parser.get_start_address());
}

auto ElfLoader::create_parser() -> ElfParser
{
    return ElfParser{image_.length, image_.bytes};
}

void ElfLoader::process_program_header(const elf32::Elf32_Phdr& header)
{
    if (elf32::SegmentType::PT_LOAD == header.p_type) {
        action_load(header);
    }
}

void ElfLoader::action_load(const elf32::Elf32_Phdr& header)
{
    // AFOX_TODO: need some way to track ownership of page frames and free them when they are no longer in use
    TRACE("begin");
    uintptr_t physical_address = page_frame_allocator_.allocate_span(header.p_memsz);
    uintptr_t linear_address = header.p_vaddr;

    // Ensure the page directory has page tables for the region of memory.
    physical_memory_map_.populate_page_tables(linear_address, header.p_memsz);

    // Map the page so the kernel can Write into it.
    TRACE("map_page(physical_address=%p, linear_address=%p, flags=physical_memory_map_.WRITABLE)",
          reinterpret_cast<char*>(physical_address),
          reinterpret_cast<char*>(linear_address));
    physical_memory_map_.map_page(physical_address, linear_address, physical_memory_map_.WRITABLE | physical_memory_map_.SUPERVISOR);
    
    // Populate the program segment. The segment in memory may be larger than
    // the data region in the file. The remainder is cleared to zero.
    memset(reinterpret_cast<char*>(linear_address), 0, header.p_memsz);
    memcpy(reinterpret_cast<char*>(linear_address),
           image_.bytes + header.p_offset,
           header.p_filesz);
    
    // Map again with the appropriate protection. This may make it read-only.
    auto flags = get_protection_flags(header);
    TRACE("map_page(physical_address=%p, linear_address=%p, flags=%d)",
          reinterpret_cast<char*>(physical_address),
          reinterpret_cast<char*>(linear_address),
          static_cast<int>(flags));
    physical_memory_map_.map_page(physical_address, linear_address, flags);

    TRACE("end");
}

ElfLoader::PhysicalMemoryMap::ProtectionFlags
ElfLoader::get_protection_flags(const elf32::Elf32_Phdr& header)
{
    ElfLoader::PhysicalMemoryMap::ProtectionFlags flags = 0;
    if (header.p_flags & elf32::PF_W) {
        flags = flags | physical_memory_map_.WRITABLE;
    }
    // AFOX_TODO: handle the executable flag too in get_protection_flags()
    return flags;
}