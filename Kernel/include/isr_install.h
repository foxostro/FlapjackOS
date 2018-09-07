#ifndef FLAPJACKOS_KERNEL_INCLUDE_ISR_INSTALL_H
#define FLAPJACKOS_KERNEL_INCLUDE_ISR_INSTALL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <idt.h>

void isr_install(IDTEntry *idt);

#ifdef __cplusplus
}
#endif

#endif // FLAPJACKOS_KERNEL_INCLUDE_ISR_INSTALL_H
