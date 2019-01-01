#include <elf_loader_factory.hpp>
#include <elf_loader32.hpp>
// #include <elf_loader64.hpp>

UniquePointer<ElfLoader>
ElfLoaderFactory::create_loader(PhysicalMemoryMap& physical_memory_map,
                                PageFrameAllocator& page_frame_allocator,
                                const Data& image)
{
    UniquePointer<ElfLoader> elf32{new ElfLoader32(physical_memory_map, page_frame_allocator, image)};
    if (elf32->can_exec()) {
        return elf32;
    }

    // UniquePointer<ElfLoader> elf64{new ElfLoader64(physical_memory_map, page_frame_allocator, image)};
    // if (elf64->is_image_acceptable()) {
    //     return elf64;
    // }

    return nullptr;
}
