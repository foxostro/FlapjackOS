#include <elf_loader_factory.hpp>
#include <elf_loader32.hpp>

UniquePointer<ElfLoader>
ElfLoaderFactory::create_loader(PhysicalMemoryMap& physical_memory_map,
                                PageFrameAllocator& page_frame_allocator,
                                const Data& image)
{
    // AFOX_TODO: Examine the image and figure out what type of loader is needed.
    return new ElfLoader32{physical_memory_map, page_frame_allocator, image};
}
