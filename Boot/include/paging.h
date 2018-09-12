#ifndef FLAPJACKOS_BOOT_INCLUDE_PAGING_H
#define FLAPJACKOS_BOOT_INCLUDE_PAGING_H

#ifndef __ASSEMBLER__
#error "This header is intended for use only in assembly code."
#endif

#include "boot.h"
#include "declare.h"

// Convert a kernel logical address to a physical memory address.
#define GET_PHYSICAL_ADDRESS(addr) ((addr) - KERNEL_VIRTUAL_START_ADDR)

#define MMU_PRESENT (1 << 0)
#define MMU_WRITABLE (1 << 1)

.set MSR_EFER, 0xC0000080
.set MSR_EFER_LME, (1 << 8)

// The kernel uses 4KB pages.
.set PAGE_SIZE, 0x1000

// Setup constants relating to hardware paging objects.
.set PML4_SIZE, 0x1000
.set PML4_ALIGNMENT, 0x1000
.set PML4_ENTRY_SIZE, 8

.set PAGE_DIRECTORY_POINTER_TABLE_SIZE, 0x1000
.set PAGE_DIRECTORY_POINTER_TABLE_ALIGNMENT, 0x1000
.set PDPT_ENTRY_SIZE, 8

.set PAGE_DIRECTORY_SIZE, 0x1000
.set PAGE_DIRECTORY_ALIGNMENT, 0x1000

.set PAGE_TABLE_SIZE, 0x1000
.set PAGE_TABLE_ALIGNMENT, 0x1000

#ifdef __x86_64__
.set PAGE_DIRECTORY_ENTRY_SIZE, 8
.set PAGE_TABLE_ENTRY_SIZE, 8
.set PAGE_TABLE_NUMBER_OF_ENTRIES, 512
#else
.set PAGE_DIRECTORY_ENTRY_SIZE, 4
.set PAGE_TABLE_ENTRY_SIZE, 4
.set PAGE_TABLE_NUMBER_OF_ENTRIES, 1024
#endif

// Allocate storage for a page directory object.
.macro DECLARE_PAGE_DIRECTORY NAME
SYMBOL \NAME, PAGE_DIRECTORY_SIZE, PAGE_DIRECTORY_ALIGNMENT
.endm

// Allocate storage for a page table object.
.macro DECLARE_PAGE_TABLE NAME
SYMBOL \NAME, PAGE_TABLE_SIZE, PAGE_TABLE_ALIGNMENT
.endm

// Allocate storage for a PML4 object.
.macro DECLARE_PML4 NAME
SYMBOL \NAME, PML4_SIZE, PML4_ALIGNMENT
.endm

// Allocate storage for a page directory pointer table object.
.macro DECLARE_PAGE_DIRECTORY_POINTER_TABLE NAME
SYMBOL \NAME, PAGE_DIRECTORY_POINTER_TABLE_SIZE, PAGE_DIRECTORY_POINTER_TABLE_ALIGNMENT
.endm

// Certain bits of a linear address are relevant for determining the index into
// each paging structure that is needed to resolve the corresponding physical
// address. The following macros extract these specific bits.
// See the Intel manual vol. 3a, sec. 4.5
#define PML4_ADDR_TO_ENTRY_INDEX(addr)           (((addr) >> 39) & 0x1FF)
#define PDPT_ADDR_TO_ENTRY_INDEX(addr)           (((addr) >> 30) & 0x1FF)
#ifdef __x86_64__
#define PAGE_DIRECTORY_ADDR_TO_ENTRY_INDEX(addr) (((addr) >> 21) & 0x1FF)
#else
#define PAGE_DIRECTORY_ADDR_TO_ENTRY_INDEX(addr) ((addr) >> 22)
#endif

// Set a PML4 entry to refer to the specified page directory pointer table.
// The mapping is always writable.
// Only considers the lower 32-bits of the linear address.
.macro SET_PML4_ENTRY PML4, PDPT, VIRTUAL_MAPPING_ADDR
movl $(\PDPT), %eax
or $(MMU_PRESENT | MMU_WRITABLE), %eax
movl %eax, \PML4 + (PML4_ADDR_TO_ENTRY_INDEX(\VIRTUAL_MAPPING_ADDR) * PML4_ENTRY_SIZE)
.endm

// Set a particular page directory pointer table entry to refer to the specified
// page directory.
// The mapping is always writable.
// Only considers the lower 32-bits of the linear address.
.macro SET_PDPT_ENTRY PDPT, PAGE_DIRECTORY, VIRTUAL_MAPPING_ADDR
movl $(\PAGE_DIRECTORY), %eax
or $(MMU_PRESENT | MMU_WRITABLE), %eax
movl %eax, \PDPT + (PDPT_ADDR_TO_ENTRY_INDEX(\VIRTUAL_MAPPING_ADDR) * PDPT_ENTRY_SIZE)
.endm

// Set a particular page directory entry to refer to the specified page table.
// The mapping is always writable.
// Only considers the lower 32-bits of the linear address.
.macro SET_PAGE_DIRECTORY_ENTRY PAGE_DIRECTORY, PAGE_TABLE, VIRTUAL_MAPPING_ADDR
movl $(\PAGE_TABLE), %eax
or $(MMU_PRESENT | MMU_WRITABLE), %eax
movl %eax, \PAGE_DIRECTORY + (PAGE_DIRECTORY_ADDR_TO_ENTRY_INDEX(\VIRTUAL_MAPPING_ADDR) * PAGE_DIRECTORY_ENTRY_SIZE)
.endm

// Fill the specified page table to establish a direct mapping.
.macro FILL_PAGE_TABLE PAGE_TABLE, FIRST_PHYSICAL_ADDRESS
movl $(\PAGE_TABLE), %edi
movl $(\FIRST_PHYSICAL_ADDRESS), %esi
movl $(PAGE_TABLE_NUMBER_OF_ENTRIES), %ecx
1:
movl %esi, %edx
orl $(MMU_PRESENT | MMU_WRITABLE), %edx
movl %edx, (%edi)
addl $(PAGE_SIZE), %esi
addl $(PAGE_TABLE_ENTRY_SIZE), %edi
loop 1b
.endm

#endif // FLAPJACKOS_BOOT_INCLUDE_PAGING_H
