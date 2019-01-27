#include <elf_loader_factory.hpp>
#include <elf_loader32.hpp>
#include <elf_loader64.hpp>

UniquePointer<ElfLoader>
ElfLoaderFactory::create_loader(PhysicalMemoryMap& physical_memory_map,
                                const Data& image)
{
    UniquePointer<ElfLoader> elf32 = new ElfLoader32(physical_memory_map, image);
    if (elf32->can_exec()) {
        return elf32;
    }

    UniquePointer<ElfLoader> elf64 = new ElfLoader64(physical_memory_map, image);
    if (elf64->can_exec()) {
        return elf64;
    }

    throw ElfLoaderException("Failed to find a loader to handle the specified image.");
}
