#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF32_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF32_HPP

#include <cstdint>
#include <cstddef>
#include "elf.hpp"

namespace elf {

// This file uses names and naming conventions from the ELF32 specification
// document, "Executable and Linkable Format (ELF), Portable Formats
// Specification, Version 1.1 Tool Interface Standards (TIS)".
// This is intentionally not congruent with the rest of Flapjack OS.


// ELF types are described in Figure 1-2, "32-Bit Data Types".
using Elf32_Addr = uint32_t;
using Elf32_Half = uint16_t;
using Elf32_Off = uint32_t;
using Elf32_Sword = int32_t;
using Elf32_Word = uint32_t;


// The ELF header is described in Figure 1-3, "ELF Header".
struct Elf32_Ehdr {
    // The initial bytes mark the file as an object file and provide
    // machine-independent data with which to decode and interpret the file's
    // contents. See also the section "ELF Identification". 
    unsigned char e_ident[EI_NIDENT];

    // Identifies the object file type.
    Elf32_Half e_type;

    // Specifies the required architecture for an individual file.
    Elf32_Half e_machine;

    // Identifies the object file version.
    Elf32_Word e_version;

    // This member gives the virtual address to which the system first transfers
    // control, thus starting the process. If the file has no associated entry
    // point, this member holds zero.
    Elf32_Addr e_entry;

    // This member holds the program header table's file offset in bytes. If the
    // file has no program header table, this member holds zero.
    Elf32_Off e_phoff;

    // This member holds the section header table's file offset in bytes. If the
    // file has no section header table, this member holds zero.
    Elf32_Off e_shoff;

    // Holds processor-specific flags associated with the file. Flag names take
    // the form EF_machine_flag. See the section "Machine Information" for
    // flag definitions.
    Elf32_Word e_flags;

    // Holds the ELF header's size in bytes.
    Elf32_Half e_ehsize;

    // Holds the size in bytes of one entry in the file's program header table;
    // all entries are the same size.
    Elf32_Half e_phentsize;

    // Holds the number of entries in the program header table. Thus the product
    // of e_phentsize and e_phnum gives the table's size in bytes. If a file has
    // no program header table, e_phnum holds the value zero.
    Elf32_Half e_phnum;

    // Holds a section header's size in bytes. A section header is one entry in
    // the section header table; all entries are the same size.
    Elf32_Half e_shentsize;

    // Holds the number of entries in the section header table. Thus the
    // product of e_shentsize and e_shnum gives the section header table's size
    // in bytes. If a file has no section header table, e_shnum holds the value
    // zero.
    Elf32_Half e_shnum;

    // Holds the section header table index of the entry associated with the
    // section name string table. If the file has no section name string table,
    // this member holds the value SHN_UNDEF. See the sections "Sections" and
    // "String Table" for more information.
    Elf32_Half e_shstrndx;
};


// An object file's section header table lets one locate the file's sections.
// Refer to section 1-9.
struct Elf32_Shdr {
    // Specifies the name of the section. Its value is an index into the section
    // header string table section, giving the location of a nul-terminated
    // string.
    Elf32_Word sh_name;

    // Categorizes the section's contents and semantics.
    Elf32_Word sh_type;

    // Sections support 1-bit flags that describe miscellaneous attributes.
    Elf32_Word sh_flags;

    // If the section will appear in the memory image of a process, this member
    // the address at which the section's first byte should reside. Otherwise,
    // the member contains 0.
    Elf32_Addr sh_addr;

    // This member's value gives the byte offset from the beginning of the file
    // to the first byte in the section. One section type, SHT_NOBITS, occupies
    // no space in the file, and its sh_offset member locates the conceptual
    // placement in the file.
    Elf32_Off sh_offset;

    // This member gives the section's size in bytes. Unless the section type is
    // SHT_NOBITS, the section occupies sh_size bytes in the file. A section of
    // type SHT_NOBITS may have a non-zero size, but it occupies no space in the
    // file.
    Elf32_Word sh_size;

    // This member holds a section header table index link, whose interpretation
    // depends on the section type.
    Elf32_Word sh_link;

    // This member holds extra information, whose interpretation depends on the
    // section type.
    Elf32_Word sh_info;

    // Some sections have address alignment constraints. Values 0 and 1 mean the
    // section has no alignment constraints.
    Elf32_Word sh_addralign;

    // Some sections hold a table of fixed-size entries, such as a symbol table.
    // For such a section, this member gives the size in bytes of each entry.
    // The member contains 0 if the section does not hold a table of fixed-size
    // entries.
    Elf32_Word sh_entsize;
 };


// An executable or shared object file's program header table is an array of
// structures, each describing a segment or other information the system needs
// to prepare the program for execution. See section 2-2.
struct Elf32_Phdr {
    // This member tells what kind of segment this array element describes or
    // how to interpret the array element's information.
    Elf32_Word p_type;

    // This member gives the offset from the beginning of the file at which the
    // first byte of the segment resides.
    Elf32_Off p_offset;

    // This member gives the virtual address at which the first byte of the
    // segment resides in memory.
    Elf32_Addr p_vaddr;

    // This member specifies the segment's physical address. However, the
    // specification declares that the contents of this member may be undefined
    // for systems which do not consider physical addressing to be relevant.
    Elf32_Addr p_paddr;

    // This member gives the number of bytes in the file image of the segment;
    // it may be zero.
    Elf32_Word p_filesz;

    // This member gives the number of bytes in the memory image of the segment;
    // it may be zero.
    Elf32_Word p_memsz;

    // This member gives flags relevant to the segment.
    Elf32_Word p_flags;

    // This member gives the value to which the segments are aligned in memory
    // and in the file. Values 0 and 1 mean no alignment is required.
    Elf32_Word p_align;
};


} // namespace elf

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF32_HPP