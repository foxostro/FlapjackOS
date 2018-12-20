#ifndef FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_INTERRUPT_DESCRIPTOR_HPP
#define FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_INTERRUPT_DESCRIPTOR_HPP

#include <cstdint>
#include <cassert>

namespace x86_64 {

// Represents the IE-32e Interrupt Descriptor on Intel CPUs.
// For details, refer to the Intel manual, volume 3a, section 6.14.
//
// Interrupt/Trap Gate:
// ------------------------------------------------------------------------------------------------
// |      63:48     | 47 | 46:45 | 44 |  43:40  | 39:35 | 34:32 |      31:16       |     15:0     |
// |----------------------------------------------------------------------------------------------|
// |  Offset 31..16 |  P |  DPL  |  0 |  Type   |   0   |  IST  | Segment Selector | Offset 15..0 |
// ------------------------------------------------------------------------------------------------
// -----------------------------
// |  63:32   |      31:0      |
// |----------------------------
// | reserved |  Offset 63..32 |
// -----------------------------
//
// where...
// P -- Present
// DPL -- Descriptor Privilege Level
// Type -- Describes the type of gate
// IST -- Index of the interrupt stack to use
// Offset -- Offset to procedure entry point
// Segment Selector -- Segment selector for destination code segment
class InterruptDescriptor {
public:
    uint64_t data;
    uint64_t data2;

    static constexpr uint64_t SEGMENT_SELECTOR_MASK = 0b0000000000000000000000000000000011111111111111110000000000000000;
    static constexpr uint64_t SEGMENT_SELECTOR_SHIFT = 16;
    static constexpr uint64_t DPL_MASK = 0b0000000000000000011000000000000000000000000000000000000000000000;
    static constexpr uint64_t DPL_SHIFT = 45;
    static constexpr unsigned PRESENT_BIT = 47;
    static constexpr uint64_t OFFSET_15_0_MASK = 0b00000000000000001111111111111111;
    static constexpr uint64_t OFFSET_31_16_MASK = 0b11111111111111110000000000000000;
    static constexpr uint64_t OFFSET_31_16_SHIFT = 48-16;
    static constexpr uint64_t OFFSET_63_32_MASK = (1ull<<32)-1;
    static constexpr uint64_t OFFSET_63_32_SHIFT = 32;

    enum GateType {
        InterruptGate,
        TrapGate
    };

    InterruptDescriptor() = default;

    void set_segment_selector(unsigned segment_selector)
    {
        assert(segment_selector < (1<<16));
        data = data & ~SEGMENT_SELECTOR_MASK;
        const uint64_t field = (segment_selector << SEGMENT_SELECTOR_SHIFT) & SEGMENT_SELECTOR_MASK;
        data = data | field;
    }

    unsigned get_segment_selector() const
    {
        return (data & SEGMENT_SELECTOR_MASK) >> SEGMENT_SELECTOR_SHIFT;
    }

    void set_type(GateType type)
    {
        switch (type) {
        case InterruptGate:
            set_bit(43);
            set_bit(42);
            set_bit(41);
            clear_bit(40);
            break;

        case TrapGate:
            set_bit(43);
            set_bit(42);
            set_bit(41);
            set_bit(40);
            break;
        }
    }

    GateType get_type() const
    {
        if (get_bit(40) == true) {
            return TrapGate;
        } else {
            return InterruptGate;
        }
    }

    void set_bit(unsigned bit_number)
    {
        assert(bit_number < 64);
        uint64_t bit = 1ull << bit_number;
        data = data | bit;
    }

    void clear_bit(unsigned bit_number)
    {
        assert(bit_number < 64);
        uint64_t bit = 1ull << bit_number;
        data = data & ~bit;
    }

    bool get_bit(unsigned bit_number) const
    {
        assert(bit_number < 64);
        return (data & (1ull << bit_number)) != 0;
    }

    void set_dpl(unsigned dpl)
    {
        assert(dpl <= 3);
        data = data & ~DPL_MASK;
        const uint64_t field = ((uint64_t)dpl << DPL_SHIFT) & DPL_MASK;
        data = data | field;
    }

    unsigned get_dpl() const
    {
        return (data & DPL_MASK) >> DPL_SHIFT;
    }

    void set_present(bool present)
    {
        if (present) {
            set_bit(PRESENT_BIT);
        } else {
            clear_bit(PRESENT_BIT);
        }
    }

    bool get_present() const
    {
        return get_bit(PRESENT_BIT);
    }

    void set_offset(uint64_t offset)
    {
        data = data & ~OFFSET_15_0_MASK;
        data = data | (offset & OFFSET_15_0_MASK);

        data = data & ~(OFFSET_31_16_MASK << OFFSET_31_16_SHIFT);
        data = data | ((offset & OFFSET_31_16_MASK) << OFFSET_31_16_SHIFT);

        data2 = data2 & ~OFFSET_63_32_MASK;
        data2 = data2 | (offset >> OFFSET_63_32_SHIFT);
    }

    uint64_t get_offset() const
    {
        return ((data2 & OFFSET_63_32_MASK) << OFFSET_63_32_SHIFT)
             | ((data & (OFFSET_31_16_MASK << OFFSET_31_16_SHIFT)) >> OFFSET_31_16_SHIFT)
             |  (data & OFFSET_15_0_MASK);
    }
};

static_assert(sizeof(InterruptDescriptor) == 16,
              "Intel manual volume 3a, section 6.14.1 specifies that an IE-32e "
              "interrupt descriptor is exactly sixteen bytes long.");

} // namespace x86_64

#endif // FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_INTERRUPT_DESCRIPTOR_HPP
