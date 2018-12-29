#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF32_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF32_HPP

#include <cstdint>
#include <cstddef>

namespace elf32 {

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


// Indexes into e_ident[] as described in Figure 1-4,
// "e_ident[ ] Identification Indexes"
constexpr size_t EI_MAG0 = 0; // File identification
constexpr size_t EI_MAG1 = 1; // File identification
constexpr size_t EI_MAG2 = 2; // File identification
constexpr size_t EI_MAG3 = 3; // File identification
constexpr size_t EI_CLASS = 4; // File class
constexpr size_t EI_DATA = 5; // Data encoding
constexpr size_t EI_VERSION = 6; // File version
constexpr size_t EI_PAD = 7; // Start of padding bytes
constexpr size_t EI_NIDENT = 16; // Size of e_ident[]


// Values for ELF file identification. See also "Elf32_Ehdr::e_ident".
// A file's first 4 bytes hold a "magic number,"" identifying the file as an
// ELF object file.
constexpr unsigned char ELFMAG0 = 0x7f; // e_ident[EI_MAG0]
constexpr unsigned char ELFMAG1 = 'E'; // e_ident[EI_MAG1]
constexpr unsigned char ELFMAG2 = 'L'; // e_ident[EI_MAG2]
constexpr unsigned char ELFMAG3 = 'F'; // e_ident[EI_MAG3]


// Values for ELF file identification. See also "Elf32_Ehdr::e_ident".
// e_ident[EI_CLASS], identifies the file's class, or capacity.
// The file format is designed to be portable among machines of various sizes,
// without imposing the sizes of the largest machine on the smallest. Class
// ELFCLASS32 supports machines with files and virtual address spaces up to 4
// gigabytes.
constexpr unsigned char ELFCLASSNONE = 0; // Invalid class
constexpr unsigned char ELFCLASS32 = 1; // 32-bit objects
constexpr unsigned char ELFCLASS64 = 2; // 64-bit objects


// Values for ELF file identification. See also "Elf32_Ehdr::e_ident".
// e_ident[EI_DATA] specifies the data encoding of the processor-specific data
// in the object file.
constexpr unsigned char ELFDATANONE = 0; // Invalid data encoding
constexpr unsigned char ELFDATA2LSB = 1; // least significant byte first
constexpr unsigned char ELFDATA2MSB = 2; // most significant byte first


// Identifies the object file type.
// The ELF types are described in 1-3.
// Although the core file contents are unspecified, type ET_CORE is reserved to
// mark the file. Values from ET_LOPROC through ET_HIPROC (inclusive) are
// reserved for processor-specific semantics. Other values are reserved and will
// be assigned to new object file types as necessary.
enum class Type : Elf32_Half {
    ET_NONE = 0, // No file type
    ET_REL = 1, // Relocatable file
    ET_EXEC = 2, // Executable file
    ET_DYN = 3, // Shared object file
    ET_CORE = 4, // Core file
    ET_LOPROC = 0xff00, // Processor-specific
    ET_HIPROC = 0xffff // Processor-specific
};

// Specifies the required architecture for an individual file.
// The ELF types are described in 1-4.
enum class Machine : Elf32_Half {
    EM_NONE = 0, // No machine
    EM_M32 = 1, // AT&T WE 32100
    EM_SPARC = 2, // SPARC
    EM_386 = 3, // Intel 80386
    EM_68K = 4, // Motorola 68000
    EM_88K = 5, // Motorola 88000
    EM_860 = 7 // Intel 80860
};

// Identifies the object file version.
// The ELF versions are described in 1-4.
enum class Version : Elf32_Word {
    EV_NONE = 0, // Invalid version
    EV_CURRENT = 1 //Current version
};


// The ELF header is described in Figure 1-3, "ELF Header".
struct Elf32_Ehdr {
    // The initial bytes mark the file as an object file and provide
    // machine-independent data with which to decode and interpret the file's
    // contents. See also the section "ELF Identification". 
    unsigned char e_ident[EI_NIDENT];

    // Identifies the object file type.
    Type e_type;

    // Specifies the required architecture for an individual file.
    Machine e_machine;

    // Identifies the object file version.
    Version e_version;

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


// This value marks an undefined, missing, irrelevant, or otherwise meaningless
// section reference. For example, a symbol "defined" relative to section number
// SHN_UNDEF is an undefined symbol.
constexpr Elf32_Half SHN_UNDEF = 0;

// This value specifies the lower bound of the range of reserved indexes.
constexpr Elf32_Half SHN_LORESERVE = 0xff00;

// Values in the inclusive range, SHN_LOPROC through SHN_HIPROC, are reserved
// for processor-specific semantics.
constexpr Elf32_Half SHN_LOPROC = 0xff00;
constexpr Elf32_Half SHN_HIPROC = 0xff1f;

// This value specifies absolute values for the corresponding reference. For
// example, symbols defined relative to section number SHN_ABS have absolute
// values and are not affected by relocation.
constexpr Elf32_Half SHN_ABS = 0xfff1;

// Symbols defined relative to this section are common symbols, such as
// FORTRAN COMMON or unallocated C external variables.
constexpr Elf32_Half SHN_COMMON = 0xfff2;

// This value specifies the upper bound of the range of reserved indexes.
// The system reserves indexes between SHN_LORESERVE and SHN_HIRESERVE,
// inclusive; the values do not reference the section header table. That is,
// the section header table does not contain entries for the reserved indexes.
constexpr Elf32_Half SHN_HIRESERVE = 0xffff;


} // namespace elf32

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF32_HPP