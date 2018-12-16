#include <platform/i386/gdt.hpp>
#include <platform/i386/lgdt.hpp>
#include <seg.h>
#include <cstring> // for memset()

namespace i386 {

void GlobalDescriptorTableEntry::init(uint32_t base,
                                      uint32_t limit,
                                      bool gr,
                                      bool sz,
                                      bool pr,
                                      unsigned privl,
                                      bool ex,
                                      bool dc,
                                      bool rw,
                                      bool ac)
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

    this->limit_0_15  = (limit & 0b00000000000000001111111111111111);
    this->base_0_15   = (base  & 0b00000000000000001111111111111111);
    this->base_16_23  = (base  & 0b00000000111111110000000000000000) >> 16;
    this->access      = access;
    this->limit_16_19 = (limit & 0b00000000000011110000000000000000) >> 16;
    this->flags       = flags;
    this->base_24_31  = (base  & 0b11111111000000000000000000000000) >> 24;
}

void GlobalDescriptorTable::establish_flat_mapping(uintptr_t tss)
{
    memset(&entries[0], 0, sizeof(entries));

    entries[0].init(0x00000000, // base
                    0x00000000, // limit
                    false,      // gr
                    false,      // sz
                    false,      // pr
                    0,          // privl
                    false,      // ex
                    false,      // dc
                    false,      // rw
                    false);     // ac

    entries[SEGSEL_KERNEL_TSS_IDX_LO].init((uint32_t)tss,// base
                                           0x67,         // limit
                                           false,        // gr
                                           false,        // sz
                                           true,         // pr
                                           0,            // privl
                                           true,         // ex
                                           false,        // dc
                                           false,        // rw
                                           true);        // ac
    entries[SEGSEL_KERNEL_TSS_IDX_LO].access = 0x89;

    entries[SEGSEL_KERNEL_CS_IDX].init(0x00000000, // base
                                       0x000fffff, // limit
                                       true,       // gr
                                       true,       // sz
                                       true,       // pr
                                       0,          // privl
                                       true,       // ex
                                       false,      // dc
                                       true,       // rw
                                       true);      // ac

    entries[SEGSEL_KERNEL_DS_IDX].init(0x00000000, // base
                                       0x000fffff, // limit
                                       true,       // gr
                                       true,       // sz
                                       true,       // pr
                                       0,          // privl
                                       false,      // ex
                                       false,      // dc
                                       true,       // rw
                                       true);      // ac

    entries[SEGSEL_USER_CS_IDX].init(0x00000000, // base
                                     0x000fffff, // limit
                                     true,       // gr
                                     true,       // sz
                                     true,       // pr
                                     3,          // privl
                                     true,       // ex
                                     false,      // dc
                                     true,       // rw
                                     true);      // ac

    entries[SEGSEL_USER_DS_IDX].init(0x00000000, // base
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

void GlobalDescriptorTable::load()
{
    lgdt(entries, sizeof(entries) - 1);
}

} // namespace i386