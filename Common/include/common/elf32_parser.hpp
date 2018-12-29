#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF32_PARSER_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF32_PARSER_HPP

#include "elf32.hpp"

namespace elf32 {

class Parser32 {
public:
    Parser32(size_t count, unsigned char* data)
     : count_(count), data_(data)
    {
        assert(count_ > sizeof(Elf32_Ehdr));
        assert(data_ != nullptr);
    }

    // Returns the ELF32 header.
    const Elf32_Ehdr& header() const
    {
        return *reinterpret_cast<const Elf32_Ehdr*>(data_);
    }

    // Returns true if the image has the expected ELF magic numbers.
    bool has_expected_magic() const
    {
        return header().e_ident[EI_MAG0] == ELFMAG0 &&
               header().e_ident[EI_MAG1] == ELFMAG1 &&
               header().e_ident[EI_MAG2] == ELFMAG2 &&
               header().e_ident[EI_MAG3] == ELFMAG3;
    }

    // Returns true if the image is marked as being appropriate for IA-32.
    // That is, appropriate for Intel 32-bit or x86.
    bool is_ia32() const
    {
        // Please refer to section 1-7, "Machine Information" for details.
        return header().e_ident[EI_CLASS] == ELFCLASS32 &&
               header().e_ident[EI_DATA] == ELFDATA2LSB && 
               header().e_machine == Machine::EM_386 &&
               header().e_flags == 0;
    }

private:
    const size_t count_;
    const unsigned char* data_;
};

} // namespace elf32

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF32_PARSER_HPP