# Macro procedure to setup a bootstrap page table.
#
# Before we enter the kernel higher half, and setup a stack, we cannot make
# function calls. Include this file in order to define the procedure inline.
#
# EDI should hold the physical address of the page table.
# ESI should hold the first page frame to map.
#
# This procedure promises not to clobber EAX and EBX (which contain values set
# by GRUB) but may clobber any other register.

    # Map 1024 pages. That's the entire page table.
    movl $1024, %ecx
1:
    # Map physical address as "present, writable" in the page table.
    movl %esi, %edx
    orl $0x003, %edx
    movl %edx, (%edi)

    # Increment the physical address. We're using 4KB pages.
    addl $4096, %esi

    # Increment to the next page table entry. Each entry is four bytes.
    addl $4, %edi
    loop 1b
