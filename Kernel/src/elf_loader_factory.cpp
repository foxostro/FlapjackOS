#include <elf_loader_factory.hpp>
#include <elf_loader32.hpp>
#include <elf_loader64.hpp>
#include <errno.h>

const char* ExecErrorDomain = "Exec";

Expected<SharedPointer<ElfLoader>>
ElfLoaderFactory::create_loader(PhysicalMemoryMap& physical_memory_map,
                                const Data& image)
{
    SharedPointer<ElfLoader> elf32 = new ElfLoader32(physical_memory_map, image);
    if (!elf32) {
        return Error{ENOMEM, ExecErrorDomain, "Could not allocate ElfLoader32"};
    }
    if (elf32->can_exec()) {
        return elf32;
    }

    SharedPointer<ElfLoader> elf64 = new ElfLoader64(physical_memory_map, image);
    if (!elf64) {
        return Error{ENOMEM, ExecErrorDomain, "Could not allocate ElfLoader64"};
    }
    if (elf64->can_exec()) {
        return elf64;
    }

    return Error{ENOEXEC, ExecErrorDomain, "Failed to find a loader to handle the specified image."};
}
