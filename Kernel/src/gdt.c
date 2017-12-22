#include <gdt.h>
#include <seg.h>

void gdt_create_entry(gdt_entry_t *entry, uint32_t base, uint32_t limit,
                      bool gr, bool sz, bool pr, unsigned privl, bool ex,
                      bool dc, bool rw, bool ac)
{
    uint8_t access = ((!!pr) << 7)
                   | ((privl & 0b11) << 5)
                   | ((pr ? 1 : 0) << 4)
                   | ((!!ex) << 3)
                   | ((!!dc) << 2)
                   | ((!!rw) << 1)
                   |  (!!ac);
    uint8_t flags = ((!!gr) << 3)
                  | ((!!sz) << 2);

    entry->limit_0_15  = (limit & 0b00000000000000001111111111111111);
    entry->base_0_15   = (base  & 0b00000000000000001111111111111111);
    entry->base_16_23  = (base  & 0b00000000111111110000000000000000) >> 16;
    entry->access      = access;
    entry->limit_16_19 = (limit & 0b00000000000011110000000000000000) >> 16;
    entry->flags       = flags;
    entry->base_24_31  = (base  & 0b11111111000000000000000000000000) >> 24;
}

void gdt_create_flat_mapping(gdt_entry_t *gdt,
                             __attribute__((unused)) size_t size,
                             uintptr_t tss)
{
    gdt_create_entry(&gdt[0],
                     0x00000000, // base
                     0x00000000, // limit
                     false,      // gr
                     false,      // sz
                     false,      // pr
                     0,          // privl
                     false,      // ex
                     false,      // dc
                     false,      // rw
                     false);     // ac

    gdt_create_entry(&gdt[SEGSEL_KERNEL_TSS_IDX],
                     (uint32_t)tss,// base
                     0x67,       // limit
                     false,      // gr
                     false,      // sz
                     true,       // pr
                     0,          // privl
                     true,       // ex
                     false,      // dc
                     false,      // rw
                     true);      // ac
    gdt[SEGSEL_KERNEL_TSS_IDX].access = 0x89;

    gdt_create_entry(&gdt[SEGSEL_KERNEL_CS_IDX],
                     0x00000000, // base
                     0x000fffff, // limit
                     true,       // gr
                     true,       // sz
                     true,       // pr
                     0,          // privl
                     true,       // ex
                     false,      // dc
                     true,       // rw
                     true);      // ac

    gdt_create_entry(&gdt[SEGSEL_KERNEL_DS_IDX],
                     0x00000000, // base
                     0x000fffff, // limit
                     true,       // gr
                     true,       // sz
                     true,       // pr
                     0,          // privl
                     false,      // ex
                     false,      // dc
                     true,       // rw
                     true);      // ac

    gdt_create_entry(&gdt[SEGSEL_USER_CS_IDX],
                     0x00000000, // base
                     0x000fffff, // limit
                     true,       // gr
                     true,       // sz
                     true,       // pr
                     3,          // privl
                     true,       // ex
                     false,      // dc
                     true,       // rw
                     true);      // ac

    gdt_create_entry(&gdt[SEGSEL_USER_DS_IDX],
                     0x00000000, // base
                     0x000fffff, // limit
                     true,       // gr
                     true,       // sz
                     true,       // pr
                     3,          // privl
                     false,      // ex
                     false,      // dc
                     true,       // rw
                     false);     // ac
}
