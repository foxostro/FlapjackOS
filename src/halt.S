.global halt_forever
halt_forever:
    hlt
    jmp halt_forever

.global hlt
hlt:
    hlt
    ret
