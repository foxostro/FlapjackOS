# Define some constants for the multiboot header.
.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

# Declare the multiboot header.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Initial stack
.align 16
stack_bottom:
.skip 16384
stack_top:

_start:
    cli
    cld

    # Setup the initial kernel stack and jump to the kernel entry point.
    leal stack_top, %esp
    mov $0, %ebp # back traces end here

    pushl %esp
    pushl %ebx
    call kernel_main  # Call the C entry point

    # Halt the machine forever. 
    cli
hang:
    hlt
    jmp hang
