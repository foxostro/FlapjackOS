#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER_FACTORY_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER_FACTORY_HPP

#include <exception>
#include <common/data.hpp>
#include <common/unique_pointer.hpp>
#include <physical_memory_map.hpp>
#include <elf_loader.hpp>

class ElfLoaderException : public std::exception
{
public:
    ElfLoaderException(const char *what) noexcept : what_(what) {}

    const char* what() const noexcept override
    {
        return what_;
    }

private:
    const char* what_;
};

// Creates an object which can load a specified ELF executable image.
class ElfLoaderFactory {
public:
    // Instantiates and returns a loader object for the specified executable.
    UniquePointer<ElfLoader>
    create_loader(PhysicalMemoryMap& physical_memory_map,
                  const Data& image);
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER_FACTORY_HPP
