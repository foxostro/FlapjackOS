#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER_FACTORY_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER_FACTORY_HPP

#include <common/data.hpp>
#include <common/expected.hpp>
#include <common/shared_pointer.hpp>
#include <physical_memory_map.hpp>
#include <elf_loader.hpp>

extern const char* ExecErrorDomain;

// Creates an object which can load a specified ELF executable image.
class ElfLoaderFactory {
public:
    // Instantiates and returns a loader object for the specified executable.
    Expected<SharedPointer<ElfLoader>>
    create_loader(PhysicalMemoryMap& physical_memory_map,
                  const Data& image);
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER_FACTORY_HPP
