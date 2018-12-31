#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF64_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF64_HPP

#include <cstdint>
#include <cstddef>

namespace elf64 {

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


// Indexes into e_ident[] as described in Table 2, "ELF Identification, e_ident"
constexpr size_t EI_MAG0 = 0; // File identification
constexpr size_t EI_MAG1 = 1; // File identification
constexpr size_t EI_MAG2 = 2; // File identification
constexpr size_t EI_MAG3 = 3; // File identification
constexpr size_t EI_CLASS = 4; // File class
constexpr size_t EI_DATA = 5; // Data encoding
constexpr size_t EI_VERSION = 6; // File version
constexpr size_t EI_OSABI = 7; // OS/ABI identification
constexpr size_t EI_ABIVERSION = 8; // ABI version
constexpr size_t EI_PAD = 9; // Start of padding bytes
constexpr size_t EI_NIDENT = 16; // Size of e_ident[]


// Values for ELF file identification. See also "Elf64_Ehdr::e_ident".
// A file's first 4 bytes hold a "magic number," identifying the file as an
// ELF object file.
constexpr unsigned char ELFMAG0 = 0x7f; // e_ident[EI_MAG0]
constexpr unsigned char ELFMAG1 = 'E'; // e_ident[EI_MAG1]
constexpr unsigned char ELFMAG2 = 'L'; // e_ident[EI_MAG2]
constexpr unsigned char ELFMAG3 = 'F'; // e_ident[EI_MAG3]


// e_ident[EI_CLASS] identifies the file's class, or capacity. This is described
// in Table 3, "Object File Classes, e_ident[EI_CLASS]".
// See also "Elf64_Ehdr::e_ident".
constexpr unsigned char ELFCLASS32 = 1; // 32-bit objects
constexpr unsigned char ELFCLASS64 = 2; // 64-bit objects


// Values for ELF file identification. See also "Elf64_Ehdr::e_ident".
// e_ident[EI_DATA] specifies the data encoding of the object file data
// structures.
// This is described in Table 4.
constexpr unsigned char ELFDATA2LSB = 1; // least significant byte first
constexpr unsigned char ELFDATA2MSB = 2; // most significant byte first


// Values for ELF file identification. See also "Elf64_Ehdr::e_ident".
// e_ident[EI_OSABI] identifies the operating system and ABI for which the
// object is prepared.
// This is described in Table 5.
constexpr unsigned char ELFOSABI_SYSV = 0; // System V ABI
constexpr unsigned char ELFOSABI_HPUX = 1; // HP-UX operating system
constexpr unsigned char ELFOSABI_STANDALONE = 1; // Standalone (embedded) application


// Values for the ELF-64 object file type.
// See also "Elf64_Ehdr::e_type".
// This is described in Table 6, "Object File Types, e_type"
const Elf64_Half ET_NONE = 0; // No file type
const Elf64_Half ET_REL = 1; // Relocatable object file
const Elf64_Half ET_EXEC = 2; // Executable file
const Elf64_Half ET_DYN = 3; // Shared object file
const Elf64_Half ET_CORE = 4; // Core file
const Elf64_Half ET_LOOS = 0xFE00; // Environment-specific use
const Elf64_Half ET_HIOS = 0xFEFF;
const Elf64_Half ET_LOPROC = 0xFF00; // Processor-specific use
const Elf64_Half ET_HIPROC = 0xFFFF;


// Values for the ELF-64 machine type field. See also "Elf64_Ehdr::e_type".
// These values are described in the document "System V Application Binary
// Interface AMD64 Architecture Processor Supplement Draft Version 0.95"
constexpr Elf64_Half EM_386 = 3; // Intel 80386, or IA-32
constexpr Elf64_Half EM_X86_64 = 62; // x86_64, or IA-32e


// See also "Elf64_Ehdr::e_version"
constexpr Elf64_Word EV_CURRENT = 1;


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


// Values for ELF-64 section indices. These are described in Table 7, "Special
// Section Indices"
constexpr size_t SHN_UNDEF = 0; // Used to mark an undefined or meaningless section reference
constexpr size_t SHN_LOPROC = 0xFF00; // Processor-specific use
constexpr size_t SHN_HIPROC = 0xFF1F;
constexpr size_t SHN_LOOS = 0xFF20; // Environment-specific use
constexpr size_t SHN_HIOS = 0xFF3F;
constexpr size_t SHN_ABS = 0xFFF1; // Indicates that the corresponding reference is an absolute value
constexpr size_t SHN_COMMON = 0xFFF2; // Indicates a symbol that has been declared as a common block (Fortran COMMON or C tentative declaration)


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


// Values for section types. See also "Elf64_Shdr::sh_type".
// This is described in Table 8, "Section Types, sh_type"
constexpr Elf64_Word SHT_NULL = 0; // Marks an unused section header
constexpr Elf64_Word SHT_PROGBITS = 1; // Contains information defined by the program
constexpr Elf64_Word SHT_SYMTAB = 2; // Contains a linker symbol table
constexpr Elf64_Word SHT_STRTAB = 3; // Contains a string table
constexpr Elf64_Word SHT_RELA = 4; // Contains “Rela” type relocation entries
constexpr Elf64_Word SHT_HASH = 5; // Contains a symbol hash table
constexpr Elf64_Word SHT_DYNAMIC = 6; // Contains dynamic linking tables
constexpr Elf64_Word SHT_NOTE = 7; // Contains note information
constexpr Elf64_Word SHT_NOBITS = 8; // Contains uninitialized space; does not occupy any space in the file
constexpr Elf64_Word SHT_REL = 9; // Contains “Rel” type relocation entries
constexpr Elf64_Word SHT_SHLIB = 10; // Reserved
constexpr Elf64_Word SHT_DYNSYM = 11; // Contains a dynamic loader symbol table
constexpr Elf64_Word SHT_LOOS = 0x60000000; // Environment-specific use
constexpr Elf64_Word SHT_HIOS = 0x6FFFFFFF;
constexpr Elf64_Word SHT_LOPROC = 0x70000000; // Processor-specific use
constexpr Elf64_Word SHT_HIPROC = 0x7FFFFFFF;


// Values for section attributes. See also "Elf64_Shdr::sh_flags".
// This is described in Table 9, "Section Attributes, sh_flags"
constexpr Elf64_Xword SHF_WRITE = 0x1; // Section contains writable data
constexpr Elf64_Xword SHF_ALLOC = 0x2; // Section is allocated in memory image of program
constexpr Elf64_Xword SHF_EXECINSTR = 0x4; // Section contains executable instructions
constexpr Elf64_Xword SHF_MASKOS = 0x0F000000; // Environment-specific use
constexpr Elf64_Xword SHF_MASKPROC = 0xF0000000; // Processor-specific use


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


// Values for segment types. See also "Elf64_Phdr::p_type".
// This is described in Table 16, "Segment Types, p_type".
constexpr Elf64_Word PT_NULL = 0; // Unused entry
constexpr Elf64_Word PT_LOAD = 1; // Loadable segment
constexpr Elf64_Word PT_DYNAMIC = 2; // Dynamic linking tables
constexpr Elf64_Word PT_INTERP = 3; // Program interpreter path name
constexpr Elf64_Word PT_NOTE = 4; // Note sections
constexpr Elf64_Word PT_SHLIB = 5; // Reserved
constexpr Elf64_Word PT_PHDR = 6; // Program header table
constexpr Elf64_Word PT_LOOS = 0x60000000; // Environment-specific use
constexpr Elf64_Word PT_HIOS = 0x6FFFFFFF;
constexpr Elf64_Word PT_LOPROC = 0x70000000; // Processor-specific use
constexpr Elf64_Word PT_HIPROC = 0x7FFFFFFF;


// Bit flags in "Elf64_Phdr::p_flags".
// This is described in Table 17, "Segment Attributes, p_flags".
constexpr Elf64_Word PF_X = 0x1; // Execute permission
constexpr Elf64_Word PF_W = 0x2; // Write permission
constexpr Elf64_Word PF_R = 0x4; // Read permission
constexpr Elf64_Word PF_MASKOS = 0x00FF0000; // These flag bits are reserved for environment-specific use
constexpr Elf64_Word PF_MASKPROC = 0xFF000000; // These flag bits are reserved for processor-specific use


} // namespace elf64

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF64_HPP