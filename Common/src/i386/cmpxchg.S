.globl cmpxchg
cmpxchg:
    pushl %ebp
    movl %esp, %ebp
    movl 16(%ebp), %ecx
    movl 12(%ebp), %eax
    movl 8(%ebp), %edx
    lock cmpxchgl %ecx, (%edx)
    sete %al
    movzbl %al, %eax
    popl %ebp
    ret
