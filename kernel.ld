ENTRY(_start)

SECTIONS
{
	. = 1M;

	PROVIDE (kernel_image_begin = .);

	/* Multi-boot header comes first, followed by the text section. */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}

	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}

	PROVIDE (kernel_image_end = .);
}
