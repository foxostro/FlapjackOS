#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <idt.h>

void isr_install(idt_entry_t *idt);

#ifdef __cplusplus
}
#endif