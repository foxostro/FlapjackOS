# Returns a pointer to the beginning of the installed IDT.
.global idt_base
idt_base:
    push   %ebp
    mov    %esp, %ebp
    sub    $0x10, %esp
    lea    -0x8(%ebp), %eax
    add    $0x2, %eax
    sidtl  (%eax)
    mov    -0x4(%ebp), %eax
    leave  
    ret

# Loads the IDTR register.
.global lidt
lidt:
    # Move parameters into registers %eax and %ecx
    movl 4(%esp), %eax # Four byte base address
    movl 8(%esp), %ecx # Two byte table limit

    # Build a six byte descriptor on the stack using the parameters, now in the
    # %eax and %cx registers. The lidt instruction wants the address of a
    # descriptor with an six byte base and a two byte limit.
    pushl %eax
    pushw %cx

    # Load the interrupt descriptor table register (IDTR)
    lidt (%esp)

    # Clean up the stack and return
    addl $6, %esp
    ret
