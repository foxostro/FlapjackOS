#ifndef FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_GDT_HPP
#define FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_GDT_HPP

#include <cstdint>
#include <cstddef>

namespace i386 {

class GlobalDescriptorTableEntry {
public:
    uint16_t limit_0_15;
    uint16_t base_0_15;
    uint8_t base_16_23;
    uint8_t access;
    uint8_t limit_16_19:4;
    uint8_t flags:4;
    uint8_t base_24_31;

    void init(uint32_t base,
              uint32_t limit,
              bool gr,
              bool sz,
              bool pr,
              unsigned privl,
              bool ex,
              bool dc,
              bool rw,
              bool ac);
};
static_assert(8 == sizeof(GlobalDescriptorTableEntry), "each GDT entry must be eight bytes");

class GlobalDescriptorTable {
public:
    GlobalDescriptorTableEntry entries[6];

    void establish_flat_mapping(uintptr_t tss);

    void load();
};

} // namespace i386

#endif // FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_GDT_HPP
