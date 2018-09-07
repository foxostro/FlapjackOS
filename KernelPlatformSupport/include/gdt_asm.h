#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_GDT_ASM_H
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_GDT_ASM_H

#ifdef __cplusplus
extern "C" {
#endif

// Loads a new GDT.
void lgdt(void *gdt, unsigned limit);

#ifdef __cplusplus
}
#endif

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_GDT_ASM_H
