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


// A section header’s sh_type member specifies the section’s semantics.
// See also "Elf32_Shdr::sh_type" and section 1-10 for details.
enum class SectionType : Elf32_Word {
    // This value marks the section header as inactive; it does not have an
    // associated section. Other members of the section header have undefined
    // values.
    SHT_NULL = 0,

    // The section holds information defined by the program, whose format and
    // meaning are determined solely by the program.
    SHT_PROGBITS = 1,

    // This section holds a symbol table.
    SHT_SYMTAB = 2,

    // The section holds a string table. An object file may have multiple string
    // table sections.
    SHT_STRTAB = 3,

    // The section holds relocation entries with explicit addends, such as type
    // Elf32_Rela for the 32-bit class of object files. An object file may have
    // multiple relocation sections.
    SHT_RELA = 4,

    // The section holds a symbol hash table.
    SHT_HASH = 5,

    // The section holds information for dynamic linking.
    SHT_DYNAMIC = 6,

    // The section holds information that marks the file in some way.
    SHT_NOTE = 7,

    // A section of this type occupies no space in the file but otherwise
    // resembles SHT_PROGBITS. Although this section contains no bytes, the
    // sh_offset member contains the conceptual file offset.
    SHT_NOBITS = 8,

    // The section holds relocation entries without explicit addends, such as
    // type Elf32_Rel for the 32-bit class of object files.
    SHT_REL = 9,

    // This section type is reserved but has unspecified semantics.
    // Programs that contain a section of this type do not conform to the ABI.
    SHT_SHLIB = 10,

    // This section holds a symbol table.
    SHT_DYNSYM = 11,

    // Values in the inclusive range [SHT_LOPROC, SHT_HIPROC] are reserved for
    // processor-specific semantics.
    SHT_LOPROC = 0x70000000,
    SHT_HIPROC = 0x7fffffff,

    // This value specifies the lower bound of the range of indexes reserved for
    // application programs.
    SHT_LOUSER = 0x80000000,

    // This value specifies the upper bound of the range of indexes reserved for
    // application programs.
    SHT_HIUSER = 0xffffffff,
};


// An object file's section header table lets one locate the file's sections.
// Refer to section 1-9.
struct Elf32_Shdr {
    // Specifies the name of the section. Its value is an index into the section
    // header string table section, giving the location of a nul-terminated
    // string.
    Elf32_Word sh_name;

    // Categorizes the section's contents and semantics.
    SectionType sh_type;

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


// Bit flags in the field "Elf32_Shdr::sh_flags".
// The section contains data that should be writable during process execution.
constexpr Elf32_Word SHF_WRITE = 0x1;

// Bit flags in the field "Elf32_Shdr::sh_flags".
// The section occupies memory during process execution. Some control sections
// do not reside in the memory image of an object file; this attribute is off
// for those sections.
constexpr Elf32_Word SHF_ALLOC = 0x2;

// Bit flags in the field "Elf32_Shdr::sh_flags".
// The section contains executable machine instructions.
constexpr Elf32_Word SHF_EXECINSTR = 0x4;

// Bit flags in the field "Elf32_Shdr::sh_flags".
// All bits included in this mask are reserved for processor-specific semantics.
constexpr Elf32_Word SHF_MASKPROC = 0xf0000000;


// Indicates the type of segment in the program header table.
// See also "Elf32_Phdr::p_type"
enum class SegmentType : Elf32_Word {
    // The array element is unused; other members' values are undefined. This
    // type lets the program header table have ignored entries.
    PT_NULL = 0,

    // The array element specifies a loadable segment, described by p_filesz and
    // p_memsz. The bytes from the file are mapped to the beginning of the
    // memory segment. If the segment's memory size (p_memsz) is larger than the
    // file size (p_filesz), the "extra" bytes are defined to hold the value 0
    // and to follow the segment's initialized area. The file size may not be
    // larger than the memory size. Loadable segment entries in the program
    // header table appear in ascending order, sorted on the p_vaddr member.
    PT_LOAD = 1,

    // Specifies dynamic linking information.
    PT_DYNAMIC = 2,

    // Specifies the location and size of a null-terminated path name to invoke
    // as an interpreter.
    PT_INTERP = 3,

    // Specifies the location and size of auxiliary information.
    PT_NOTE = 4,

    // This segment type is reserved but has unspecified semantics. Programs
    // that contain an array element of this type do not conform to the ABI.
    PT_SHLIB = 5,

    // The array element, if present, specifies the location and size of the
    // program header table itself, both in the file and in the memory image of
    // the program.
    PT_PHDR = 6,

    // Values in the inclusive range [PT_LOPROC, PT_HIPROC] are reserved for
    // processor-specific semantics.
    PT_LOPROC = 0x70000000,
    PT_HIPROC = 0x7fffffff
};


// An executable or shared object file's program header table is an array of
// structures, each describing a segment or other information the system needs
// to prepare the program for execution. See section 2-2.
struct Elf32_Phdr {
    // This member tells what kind of segment this array element describes or
    // how to interpret the array element's information.
    SegmentType p_type;

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


} // namespace elf32

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_ELF32_HPP