#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF64_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF64_HPP

#include <cstdint>
#include <cstddef>
#include "elf.hpp"

namespace elf {

// This file uses names and naming conventions from the ELF64 specification
// document, "ELF-64 Object File Format, Version 1.5, Draft 2, May 27, 1998".
// This is intentionally not congruent with the rest of Flapjack OS.


// ELF types are described in Table 1, "ElF-64 Data Types".
using Elf64_Addr = uint64_t;
using Elf64_Off = uint64_t;
using Elf64_Half = uint16_t;
using Elf64_Word = uint32_t;
using Elf64_Sword = int32_t;
using Elf64_Xword = uint64_t;
using Elf64_Sxword = int64_t;


// The ELF header is described in Figure 2, "ELF-64 Header".
struct Elf64_Ehdr {
    // Identify the file as an ELF object file, and provide information about
    // the data representation of the object file structures.
    unsigned char e_ident[EI_NIDENT];

    // Identifies the object file type.
    Elf64_Half e_type;

    // Specifies the required architecture for an individual file.
    Elf64_Half e_machine;

    // Identifies the object file version. Currently, this field has the value
    // EV_CURRENT. See page 4.
    Elf64_Word e_version;

    // Contains the virtual address of the program entry point. If there is no
    // entry point, this field contains zero.
    Elf64_Addr e_entry;

    // Contains the file offset, in bytes, of the program header table.
    Elf64_Off e_phoff;

    // Contains the file offset, in bytes, of the section header table.
    Elf64_Off e_shoff;

    // Contains processor-specific flags.
    Elf64_Word e_flags;

    // Contains the size, in bytes, of the ELF header.
    Elf64_Half e_ehsize;

    // Contains the size, in bytes, of a program header table entry.
    Elf64_Half e_phentsize;

    // Contains the number of entries in the program header table.
    Elf64_Half e_phnum;

    // Contains the size, in bytes, of a section header table entry.
    Elf64_Half e_shentsize;

    // Contains the number of entries in the section header table.
    Elf64_Half e_shnum;

    // Contains the section header table index of the section containing the
    // section name string table. If there is no section name string table, this
    // field has the value SHN_UNDEF.
    Elf64_Half e_shstrndx;
};


// An object file's section header table lets one locate the file's sections.
// Refer to Section 4, Figure 3, "ELF-64 Section Header".
struct Elf64_Shdr {
    // Contains the offset, in bytes, to the section name, relative to the start
    // of the section name string table.
    Elf64_Word sh_name;

    // Identifies the section type. Table 8 lists the processor-independent
    // values for this field.
    Elf64_Word sh_type;

    // Identifies the attributes of the section. Table 9 lists the processor-
    // independent values for these flags.
    Elf64_Xword sh_flags;

    // Contains the virtual address of the beginning of the section in memory.
    // If the section is not allocated to the memory image of the program, this
    // field should be zero.
    Elf64_Addr sh_addr;

    // Contains the offset, in bytes, of the beginning of the section contents
    // in the file.
    Elf64_Off sh_offset;

    // Contains the size, in bytes, of the section. Except for SHT_NOBITS
    // sections, this is the amount of space occupied in the file.
    Elf64_Xword sh_size;

    // Contains the section index of an associated section. This field is used
    // for several purposes, depending on the type of section, as explained in
    // Table 10.
    Elf64_Word sh_link;

    // Contains extra information about the section. This field is used for
    // several purposes, depending on the type of section, as explained in
    // Table 11.
    Elf64_Word sh_info;

    // Contains the required alignment of the section. This field must be a
    // power of two.
    Elf64_Xword sh_addralign;

    // Contains the size, in bytes, of each entry, for sections that contain
    // fixed-size entries. Otherwise, this field contains zero.
    Elf64_Xword sh_entsize;
};


// An executable or shared object file's program header table is an array of
// structures, each describing a segment or other information the system needs
// to prepare the program for execution. See section 8, "Program header table"
// and Figure 6, "ELF-64 Program Header Table Entry"
struct Elf64_Phdr {
    // Identifies the type of segment. The processor-independent segment types
    // are shown in Table 16.
    Elf64_Word p_type;

    // Contains the segment attributes. The processor-independent flags are
    // shown in Table 17. The top eight bits are reserved for processor-specific
    // use, and the next eight bits are reserved for environment-specific use.
    Elf64_Word p_flags;

    // Contains the offset, in bytes, of the segment from the beginning of the
    // file.
    Elf64_Off p_offset;

    // Contains the virtual address of the segment in memory.
    Elf64_Addr p_vaddr;

    // Field is reserved for systems with physical addressing.
    Elf64_Addr p_paddr;

    // Contains the size, in bytes, of the file image of the segment.
    Elf64_Xword p_filesz;

    // Contains the size, in bytes, of the memory image of the segment.
    Elf64_Xword p_memsz;

    // Specifies the alignment constraint for the segment.
    Elf64_Xword p_align;
};


} // namespace elf

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF64_HPP