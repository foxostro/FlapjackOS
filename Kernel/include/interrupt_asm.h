#ifndef FLAPJACKOS_KERNEL_INCLUDE_INTERRUPT_ASM_H
#define FLAPJACKOS_KERNEL_INCLUDE_INTERRUPT_ASM_H

#ifdef __cplusplus
extern "C" {
#endif

void disable_interrupts(void);
void enable_interrupts(void);

#ifdef __cplusplus
}
#endif

#endif // FLAPJACKOS_KERNEL_INCLUDE_INTERRUPT_ASM_H
