#include <idt_defines.h>

.global asm_de_wrapper
asm_de_wrapper:
    cli
    pusha
    pushl $IDT_DE
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global asm_db_wrapper
asm_db_wrapper:
    cli
    pusha
    pushl $IDT_DB
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global asm_nmi_wrapper
asm_nmi_wrapper:
    cli
    pusha
    pushl $IDT_NMI
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    iret

.global asm_bp_wrapper
asm_bp_wrapper:
    cli
    pusha
    pushl $IDT_BP
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global asm_of_wrapper
asm_of_wrapper:
    cli
    pusha
    pushl $IDT_OF
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global asm_br_wrapper
asm_br_wrapper:
    cli
    pusha
    pushl $IDT_BR
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global asm_ud_wrapper
asm_ud_wrapper:
    cli
    pusha
    pushl $IDT_UD
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global asm_nm_wrapper
asm_nm_wrapper:
    cli
    pusha
    pushl $IDT_NM
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global asm_df_wrapper
asm_df_wrapper:
    cli
    pusha
    pushl $IDT_DF
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    addl $4, %esp  # Clean up the error code.
    sti
    iret

.global asm_cso_wrapper
asm_cso_wrapper:
    cli
    pusha
    pushl $IDT_CSO
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global asm_ts_wrapper
asm_ts_wrapper:
    cli
    pusha
    pushl $IDT_TS
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    addl $4, %esp  # Clean up the error code.
    sti
    iret

.global asm_np_wrapper
asm_np_wrapper:
    cli
    pusha
    pushl $IDT_NP
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    addl $4, %esp  # Clean up the error code.
    sti
    iret

.global asm_ss_wrapper
asm_ss_wrapper:
    cli
    pusha
    pushl $IDT_SS
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    addl $4, %esp  # Clean up the error code.
    sti
    iret

.global asm_gp_wrapper
asm_gp_wrapper:
    cli
    pusha
    pushl $IDT_GP
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    addl $4, %esp  # Clean up the error code.
    sti
    iret

.global asm_pf_wrapper
asm_pf_wrapper:
    cli
    pusha
    pushl $IDT_PF
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    addl $4, %esp  # Clean up the error code.
    sti
    iret

.global asm_mf_wrapper
asm_mf_wrapper:
    cli
    pusha
    pushl $IDT_MF
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global asm_ac_wrapper
asm_ac_wrapper:
    cli
    pusha
    pushl $IDT_AC
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global asm_mc_wrapper
asm_mc_wrapper:
    cli
    pusha
    pushl $IDT_MC
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global asm_xf_wrapper
asm_xf_wrapper:
    cli
    pusha
    pushl $IDT_XF
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global asm_timer_wrapper
asm_timer_wrapper:
    cli
    pusha
    pushl $TIMER_IDT_ENTRY
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    iret

.global asm_keyboard_wrapper
asm_keyboard_wrapper:
    cli
    pusha
    pushl $KEY_IDT_ENTRY
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    iret

.global enable_interrupts
enable_interrupts:
	sti
	ret

.global disable_interrupts
disable_interrupts:
    cli
    ret
