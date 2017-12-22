#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Loads a new GDT.
void lgdt(void *gdt, unsigned limit);

#ifdef __cplusplus
}
#endif
