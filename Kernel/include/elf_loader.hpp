#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER_HPP

// Interface for a class which can load an ELF image into memory for exec().
class ElfLoader {
public:
    // Load the executable image into the current address space.
    // Then, execute at the specified start address.
    virtual unsigned exec() = 0;
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_LOADER_HPP
