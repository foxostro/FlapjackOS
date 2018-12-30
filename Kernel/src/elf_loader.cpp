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
    Data segment_data = get_segment_data(header);
    uintptr_t physical_address = page_frame_allocator_.allocate_span(segment_data.length);
    uintptr_t linear_address = header.p_vaddr;
    auto flags = get_protection_flags(header);
    physical_memory_map_.map_page(physical_address, linear_address, flags);
    memmove(reinterpret_cast<char*>(linear_address),
            segment_data.bytes,
            segment_data.length);
}

Data ElfLoader::get_segment_data(const elf32::Elf32_Phdr& header)
{
    Data segment_data;
    segment_data.bytes = image_.bytes + header.p_offset;
    segment_data.length = header.p_memsz;
    return segment_data;
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