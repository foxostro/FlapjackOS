#ifndef FLAPJACKOS_BOOT_INCLUDE_CREG_BITS_H
#define FLAPJACKOS_BOOT_INCLUDE_CREG_BITS_H

// The abbreviated names of bits comes from the Intel manual vol. 3a, sec. 2.5.

#define CR0_PE (1 << 0)  // protection enable
#define CR0_MP (1 << 1)  // monitor coprocessor
#define CR0_EM (1 << 2)  // emulation
#define CR0_TS (1 << 3)  // task switched
#define CR0_ET (1 << 4)  // extension type
#define CR0_NE (1 << 5)  // numeric error
#define CR0_WP (1 << 16) // write protect
#define CR0_AM (1 << 18) // alignment mask
#define CR0_NW (1 << 29) // not write-through
#define CR0_CD (1 << 30) // cache disable
#define CR0_PG (1 << 31) // paging

#define CR4_VME        (1 << 0)  // virtual 8086 mode extensions
#define CR4_PVI        (1 << 1)  // protected mode virtual interrupts
#define CR4_TSD        (1 << 2)  // time stamp disable
#define CR4_DE         (1 << 3)  // debugging extensions
#define CR4_PSE        (1 << 4)  // page size extension
#define CR4_PAE        (1 << 5)  // physical address extension
#define CR4_MCE        (1 << 6)  // machine check exception
#define CR4_PGE        (1 << 7)  // page global enable
#define CR4_PCE        (1 << 8)  // performance monitoring counter enable
#define CR4_OSFXSR     (1 << 9)  // os support for fxsave and fxrstor instructions
#define CR4_OSXMMEXCPT (1 << 10) // os support for unmasked simd floating point exceptions
#define CR4_UMIP       (1 << 11) // user mode instruction prevention (#GP on SGDT, SIDT, SLDT, SMSW, and STR instructions when CPL > 0)
#define CR4_VMXE       (1 << 13) // virtual machine extensions enable
#define CR4_SMXE       (1 << 14) // safer mode extensions enable
#define CR4_PCIDE      (1 << 17) // pcid enable
#define CR4_OSXSAVE    (1 << 18) // xsave and processor extended states enable
#define CR4_SMEP       (1 << 20) // supervisor mode executions protection enable
#define CR4_SMAP       (1 << 21) // supervisor mode access protection enable

#endif // FLAPJACKOS_BOOT_INCLUDE_CREG_BITS_H
