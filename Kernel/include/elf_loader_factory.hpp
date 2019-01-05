#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER_FACTORY_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER_FACTORY_HPP

#include <common/data.hpp>
#include <common/unique_pointer.hpp>
#include <kernel_policy.hpp>
#include <page_frame_allocator.hpp>
#include <elf_loader.hpp>

// Creates an object which can load a specified ELF executable image.
class ElfLoaderFactory : private KernelPolicy {
public:
    // Instantiates and returns a loader object for the specified executable.
    UniquePointer<ElfLoader>
    create_loader(EarlyPhysicalMemoryMap& physical_memory_map,
                  PageFrameAllocator& page_frame_allocator,
                  const Data& image);
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER_FACTORY_HPP
