#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_HPP

#include <cstdint>
#include <cstddef>

namespace Elf {

// This file uses names and naming conventions from the ELF64 specification
// document, "ELF-64 Object File Format, Version 1.5, Draft 2, May 27, 1998".
// This is intentionally not congruent with the rest of Flapjack OS.


// Indexes into e_ident[] as described in Table 2, "ELF Identification, e_ident"
constexpr unsigned EI_MAG0 = 0; // File identification
constexpr unsigned EI_MAG1 = 1; // File identification
constexpr unsigned EI_MAG2 = 2; // File identification
constexpr unsigned EI_MAG3 = 3; // File identification
constexpr unsigned EI_CLASS = 4; // File class
constexpr unsigned EI_DATA = 5; // Data encoding
constexpr unsigned EI_VERSION = 6; // File version
constexpr unsigned EI_OSABI = 7; // OS/ABI identification
constexpr unsigned EI_ABIVERSION = 8; // ABI version
constexpr unsigned EI_PAD = 9; // Start of padding bytes
constexpr unsigned EI_NIDENT = 16; // Size of e_ident[]


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
const unsigned ET_NONE = 0; // No file type
const unsigned ET_REL = 1; // Relocatable object file
const unsigned ET_EXEC = 2; // Executable file
const unsigned ET_DYN = 3; // Shared object file
const unsigned ET_CORE = 4; // Core file
const unsigned ET_LOOS = 0xFE00; // Environment-specific use
const unsigned ET_HIOS = 0xFEFF;
const unsigned ET_LOPROC = 0xFF00; // Processor-specific use
const unsigned ET_HIPROC = 0xFFFF;


// Values for the ELF-64 machine type field. See also "Elf64_Ehdr::e_type".
// These values are described in the document "System V Application Binary
// Interface AMD64 Architecture Processor Supplement Draft Version 0.95"
constexpr unsigned EM_386 = 3; // Intel 80386, or IA-32
constexpr unsigned EM_X86_64 = 62; // x86_64, or IA-32e


// See also "Elf64_Ehdr::e_version"
constexpr unsigned EV_CURRENT = 1;


// Values for ELF-64 section indices. These are described in Table 7, "Special
// Section Indices"
constexpr unsigned SHN_UNDEF = 0; // Used to mark an undefined or meaningless section reference
constexpr unsigned SHN_LOPROC = 0xFF00; // Processor-specific use
constexpr unsigned SHN_HIPROC = 0xFF1F;
constexpr unsigned SHN_LOOS = 0xFF20; // Environment-specific use
constexpr unsigned SHN_HIOS = 0xFF3F;
constexpr unsigned SHN_ABS = 0xFFF1; // Indicates that the corresponding reference is an absolute value
constexpr unsigned SHN_COMMON = 0xFFF2; // Indicates a symbol that has been declared as a common block (Fortran COMMON or C tentative declaration)


// Values for section types. See also "Elf64_Shdr::sh_type".
// This is described in Table 8, "Section Types, sh_type"
constexpr unsigned SHT_NULL = 0; // Marks an unused section header
constexpr unsigned SHT_PROGBITS = 1; // Contains information defined by the program
constexpr unsigned SHT_SYMTAB = 2; // Contains a linker symbol table
constexpr unsigned SHT_STRTAB = 3; // Contains a string table
constexpr unsigned SHT_RELA = 4; // Contains “Rela” type relocation entries
constexpr unsigned SHT_HASH = 5; // Contains a symbol hash table
constexpr unsigned SHT_DYNAMIC = 6; // Contains dynamic linking tables
constexpr unsigned SHT_NOTE = 7; // Contains note information
constexpr unsigned SHT_NOBITS = 8; // Contains uninitialized space; does not occupy any space in the file
constexpr unsigned SHT_REL = 9; // Contains “Rel” type relocation entries
constexpr unsigned SHT_SHLIB = 10; // Reserved
constexpr unsigned SHT_DYNSYM = 11; // Contains a dynamic loader symbol table
constexpr unsigned SHT_LOOS = 0x60000000; // Environment-specific use
constexpr unsigned SHT_HIOS = 0x6FFFFFFF;
constexpr unsigned SHT_LOPROC = 0x70000000; // Processor-specific use
constexpr unsigned SHT_HIPROC = 0x7FFFFFFF;


// Values for section attributes. See also "Elf64_Shdr::sh_flags".
// This is described in Table 9, "Section Attributes, sh_flags"
constexpr unsigned SHF_WRITE = 0x1; // Section contains writable data
constexpr unsigned SHF_ALLOC = 0x2; // Section is allocated in memory image of program
constexpr unsigned SHF_EXECINSTR = 0x4; // Section contains executable instructions
constexpr uint64_t SHF_MASKOS = 0x0F000000; // Environment-specific use
constexpr uint64_t SHF_MASKPROC = 0xF0000000; // Processor-specific use


// Values for segment types. See also "Elf64_Phdr::p_type".
// This is described in Table 16, "Segment Types, p_type".
constexpr unsigned PT_NULL = 0; // Unused entry
constexpr unsigned PT_LOAD = 1; // Loadable segment
constexpr unsigned PT_DYNAMIC = 2; // Dynamic linking tables
constexpr unsigned PT_INTERP = 3; // Program interpreter path name
constexpr unsigned PT_NOTE = 4; // Note sections
constexpr unsigned PT_SHLIB = 5; // Reserved
constexpr unsigned PT_PHDR = 6; // Program header table
constexpr unsigned PT_LOOS = 0x60000000; // Environment-specific use
constexpr unsigned PT_HIOS = 0x6FFFFFFF;
constexpr unsigned PT_LOPROC = 0x70000000; // Processor-specific use
constexpr unsigned PT_HIPROC = 0x7FFFFFFF;


// Bit flags in "Elf64_Phdr::p_flags".
// This is described in Table 17, "Segment Attributes, p_flags".
constexpr unsigned PF_X = 0x1; // Execute permission
constexpr unsigned PF_W = 0x2; // Write permission
constexpr unsigned PF_R = 0x4; // Read permission
constexpr unsigned PF_MASKOS = 0x00FF0000; // These flag bits are reserved for environment-specific use
constexpr unsigned PF_MASKPROC = 0xFF000000; // These flag bits are reserved for processor-specific use


constexpr unsigned STT_FUNC = 2;


} // namespace Elf

#define ELF32_ST_TYPE(i) ((i)&0xf)
#define ELF64_ST_TYPE(i) ((i)&0xf)

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF_HPP