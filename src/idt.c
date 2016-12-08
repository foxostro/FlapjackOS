#include <idt.h>
#include <misc.h>

// Useful for installing Trap Gates
#define DPL_FIELD(DPL)   (unsigned)( ((DPL) & 0x03) << 13 )
#define TRAP_GATE_WORD_1 0x8F00

#define SIZEOF_IDT_ENTRY (8) // IDT entry is 8 bytes long (64 bits)

void idt_install_trap_gate(unsigned index, void *pOffset, unsigned DPL)
{
	unsigned offset = (unsigned)(pOffset);

	// Construct the IDT entry according to the Intel documentation for a
	// "Trap Gate" entry. (See page 151 of intel-sys.pdf for details.)
	unsigned short *entry_words = (unsigned short *) ((char *)idt_base() + (index * SIZEOF_IDT_ENTRY));

	entry_words[0] = DWORD_LOWER_WORD(offset);
	entry_words[1] = SEGSEL_KERNEL_CS;
	entry_words[2] = TRAP_GATE_WORD_1 | DPL_FIELD(DPL);
	entry_words[3] = DWORD_UPPER_WORD(offset);
}

static void idt_reset()
{
	for (int i = 0; i < IDT_ENTS; ++i) {
		idt_install_trap_gate(i, unused_hardware_interrupt, 0);
	}
}
