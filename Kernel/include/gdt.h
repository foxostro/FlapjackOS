#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <gdt_asm.h>

typedef struct {
    uint16_t limit_0_15;
    uint16_t base_0_15;
    uint8_t base_16_23;
    uint8_t access;
    uint8_t limit_16_19:4;
    uint8_t flags:4;
    uint8_t base_24_31;
} gdt_entry_t;

#ifdef __cplusplus
static_assert(8 == sizeof(gdt_entry_t), "each GDT entry is eight bytes");
#else
_Static_assert(8 == sizeof(gdt_entry_t), "each GDT entry is eight bytes");
#endif

void gdt_create_entry(gdt_entry_t *entry, uint32_t base, uint32_t limit,
                      bool gr, bool sz, bool pr, unsigned privl, bool ex,
                      bool dc, bool rw, bool ac);

// Loads a four entry GDT that establishes a basic flat mapping.
void gdt_create_flat_mapping(gdt_entry_t *gdt, size_t size, uintptr_t tss);

#ifdef __cplusplus
}
#endif