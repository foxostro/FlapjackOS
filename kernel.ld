KERNEL_VIRTUAL_START_ADDR = 0xC0000000;
ONE_MEGABYTE = 0x100000;

ENTRY(_start)

SECTIONS
{
	. = KERNEL_VIRTUAL_START_ADDR + ONE_MEGABYTE;

	PROVIDE (kernel_image_begin = .);

	/* Multi-boot header comes first, followed by the text section. */
	.text ALIGN(4K) : AT(ADDR(.text)-KERNEL_VIRTUAL_START_ADDR)
	{
		*(.multiboot)
		*(.text)
	}

	.rodata ALIGN(4K) : AT(ADDR(.rodata)-KERNEL_VIRTUAL_START_ADDR)
	{
		start_ctors = .;
		*(SORT(.ctors*))  /* Note the "SORT" */
		end_ctors = .;

		start_dtors = .;
		*(SORT(.dtors*))
		end_dtors = .;

		kernel_virtual_addr_start = KERNEL_VIRTUAL_START_ADDR;

		*(.rodata)
	}

	.data ALIGN(4K) : AT(ADDR(.data)-KERNEL_VIRTUAL_START_ADDR)
	{
		*(.data)
	}

	.bss ALIGN(4K) : AT(ADDR(.bss)-KERNEL_VIRTUAL_START_ADDR)
	{
		*(COMMON)
		*(.bss)
	}

	PROVIDE (kernel_image_end = .);
}
