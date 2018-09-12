# These are utility routines related to interrupt handling.

.global enable_interrupts
enable_interrupts:
    sti
    retq

.global disable_interrupts
disable_interrupts:
    cli
    retq
