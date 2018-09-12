#include "boot.h"

ENTRY(_start)

SECTIONS
{
    . = KERNEL_PHYSICAL_LOAD_ADDR;
    g_kernel_physical_start = .;
    g_kernel_image_begin = . + KERNEL_VIRTUAL_START_ADDR;

    .boot :
    {
        *(.multiboot)
        . = ALIGN(16);
        *(.boottext)
        *(.bootrodata)
        *(.bootdata)
        *(.bootbss)
    }

    . = ALIGN(4K);
    g_boot_end = .;

    . += KERNEL_VIRTUAL_START_ADDR;

    .text : AT(g_boot_end)
    {
        *(.text)
    }

    . = ALIGN(4K);
    g_kernel_text_end = .;

    .rodata : AT(ADDR(.rodata) - KERNEL_VIRTUAL_START_ADDR)
    {
        g_start_ctors = .;
        *(SORT(.ctors*))  /* Note the "SORT" */
        g_end_ctors = .;

        g_start_dtors = .;
        *(SORT(.dtors*))
        g_end_dtors = .;

        *(.rodata)
    }

    . = ALIGN(4K);
    g_kernel_rodata_end = .;

    .data : AT(ADDR(.data) - KERNEL_VIRTUAL_START_ADDR)
    {
        *(.data)
    }

    . = ALIGN(4K);
    g_kernel_data_end = .;

    .bss : AT(ADDR(.bss) - KERNEL_VIRTUAL_START_ADDR)
    {
        *(COMMON)
        *(.bss)
    }

    . = ALIGN(4K);
    g_kernel_image_end = .;
    g_kernel_physical_end = . - KERNEL_VIRTUAL_START_ADDR;

    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame)
    }
}
