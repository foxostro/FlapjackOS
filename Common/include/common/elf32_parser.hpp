#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF32_PARSER_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF32_PARSER_HPP

#include "elf32.hpp"

namespace elf {

class Parser32 {
public:
    Parser32(size_t count, unsigned char* data)
     : count_(count), data_(data)
    {
        assert(count_ > sizeof(Elf32_Ehdr));
        assert(data_ != nullptr);
    }

    // Returns true if the image is marked as being appropriate for IA-32.
    // That is, appropriate for Intel 32-bit or x86.
    bool is_ia32() const
    {
        // Please refer to section 1-7, "Machine Information" for details.
        return has_expected_magic() &&
               is_header_size_valid() &&
               is_version_valid() &&
               header().e_ident[EI_CLASS] == ELFCLASS32 &&
               header().e_ident[EI_DATA] == ELFDATA2LSB && 
               header().e_machine == EM_386 &&
               header().e_flags == 0;
    }

    // Returns true if the image is marked as being appropriate for IA-32e.
    // That is, appropriate for Intel 64-bit or x86_64.
    bool is_ia32e() const
    {
        return false;
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

    // Check that the header size is as expected.
    bool is_header_size_valid() const
    {
        return header().e_ehsize == sizeof(Elf32_Ehdr);
    }

    // Checks that the ELF image version is as expected.
    bool is_version_valid() const
    {
        return header().e_version == EV_CURRENT &&
               header().e_ident[EI_VERSION] == EV_CURRENT;
    }

    // Returns true if the image is for an executable file.
    // This may return false for libraries, for example.
    bool is_executable() const
    {
        return header().e_type == ET_EXEC;
    }

    // Gets the start address for the executable, or zero.
    uintptr_t get_start_address() const
    {
        return static_cast<uintptr_t>(header().e_entry);
    }

    // Returns a reference to the specified section header.
    const Elf32_Shdr& get_section_header(size_t index) const
    {
        assert(index < get_number_of_section_headers());
        assert(is_section_header_size_valid());
        const Elf32_Shdr* section_headers = get_section_headers();
        const Elf32_Shdr& section_header = section_headers[index];
        return section_header;
    }

    // Returns the number of section headers defined in this image.
    size_t get_number_of_section_headers() const
    {
        return header().e_shnum;
    }

    // Returns a pointer to the section header table.
    const Elf32_Shdr* get_section_headers() const
    {
        assert(is_section_header_size_valid());
        return reinterpret_cast<const Elf32_Shdr*>(data_ + header().e_shoff);
    }

    // Returns true if e_shentsize does match the size of a section header.
    bool is_section_header_size_valid() const
    {
        return header().e_shentsize == sizeof(Elf32_Shdr);
    }

    const char* get_section_name(size_t index) const
    {
        return get_section_name_string_table() + index;
    }

    const char* get_section_name_string_table() const
    {
        const Elf32_Shdr& section = get_section_name_string_table_section();
        return reinterpret_cast<const char*>(data_ + section.sh_offset);
    }

    const Elf32_Shdr& get_section_name_string_table_section() const
    {
        assert(header().e_shstrndx != SHN_UNDEF);
        const Elf32_Shdr& section = get_section_header(header().e_shstrndx);
        assert(section.sh_type == SHT_STRTAB);
        assert(section.sh_size > 0);
        return section;
    }

    // Returns a reference to the specified program header.
    const Elf32_Phdr& get_program_header(size_t index) const
    {
        assert(index < get_number_of_program_headers());
        assert(is_program_header_size_valid());
        const Elf32_Phdr* program_headers = get_program_headers();
        const Elf32_Phdr& program_header = program_headers[index];
        return program_header;
    }

    // Returns the number of program headers defined in this image.
    size_t get_number_of_program_headers() const
    {
        return header().e_phnum;
    }

    // Returns a pointer to the program header table.
    const Elf32_Phdr* get_program_headers() const
    {
        assert(is_program_header_size_valid());
        return reinterpret_cast<const Elf32_Phdr*>(data_ + header().e_phoff);
    }

    // Returns true if e_phentsize does match the size of a program header.
    bool is_program_header_size_valid() const
    {
        return header().e_phentsize == sizeof(Elf32_Phdr);
    }

    // Enumerate the program headers in the ELF image.
    template<typename Function>
    void enumerate(Function&& fn)
    {
        for (size_t i = 0, n = get_number_of_program_headers(); i < n; ++i) {
            fn(get_program_header(i));
        }
    }

private:
    size_t count_;
    unsigned char* data_;
};

} // namespace elf

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF32_PARSER_HPP