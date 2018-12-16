.global inb
inb:
    xorq %rax, %rax
    movw %di, %dx # The I/O port address was passed in %rdi
    in %dx, %al
    retq

.global inw
inw:
    xorq %rax, %rax
    movw %di, %dx # The I/O port address was passed in %rdi
    in %dx, %ax
    retq

.global ind
ind:
    xorq %rax, %rax
    movw %di, %dx # The I/O port address was passed in %rdi
    in %dx, %eax
    retq

.global outb
outb:
    # The first parameter was passed in %rdi.
    # This is the I/O port address.
    movw %di, %dx

    # The second parameter was passed in %rsi.
    # We write one byte from this into the I/O port.
    movq %rsi, %rax

    out %al, %dx
    retq

.global outw
outw:
    # The first parameter was passed in %rdi.
    # This is the I/O port address.
    movw %di, %dx

    # The second parameter was passed in %rsi.
    # We write two bytes from this into the I/O port.
    movq %rsi, %rax

    out %ax, %dx
    retq

.global outd
outd:
    # The first parameter was passed in %rdi.
    # This is the I/O port address.
    movw %di, %dx

    # The second parameter was passed in %rsi.
    # We write four bytes from this into the I/O port.
    movq %rsi, %rax

    out %eax, %dx
    retq
