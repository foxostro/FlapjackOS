// Returns a pointer to the beginning of the installed IDT.
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
