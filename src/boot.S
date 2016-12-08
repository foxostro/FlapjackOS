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

# Setup a 16KiB stack with 16 byte alignment.
.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text
.global _start
.type _start, @function
_start:
	# Setup the initial kernel stack and jump to the kernel entry point.
	mov $stack_top, %esp
	call kernelMain

	# Halt the machine forever.	
	cli
1:	hlt
	jmp 1b

.size _start, . - _start
