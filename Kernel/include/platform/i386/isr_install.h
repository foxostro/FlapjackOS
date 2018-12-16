#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_ISR_INSTALL_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_ISR_INSTALL_HPP

#ifdef __cplusplus
extern "C" {
#endif

#include "idt.h"

void isr_install(IDTEntry *idt);

#ifdef __cplusplus
}
#endif

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_ISR_INSTALL_HPP
