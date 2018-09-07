#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_INTERRUPT_ASM_H
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_INTERRUPT_ASM_H

#ifdef __cplusplus
extern "C" {
#endif

void disable_interrupts(void);
void enable_interrupts(void);

#ifdef __cplusplus
}
#endif

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_INTERRUPT_ASM_H
