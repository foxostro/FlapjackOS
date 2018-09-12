.global get_cr2
get_cr2:
    mov %cr2, %rax
    retq

.global get_cr3
get_cr3:
    mov %cr3, %rax
    retq
