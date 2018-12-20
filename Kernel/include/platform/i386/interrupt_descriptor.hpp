#ifndef FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_INTERRUPT_DESCRIPTOR_HPP
#define FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_INTERRUPT_DESCRIPTOR_HPP

#include <cstdint>
#include <cassert>

namespace i386 {

// Represents the IE-32 Interrupt Descriptor on Intel CPUs.
// For details, refer to the Intel manual, volume 3a, section 6.11.
//
// Task Gate:
// -----------------------------------------------------------------------------------
// |   63:48   | 47 | 46:45 |   44:40   |   39:32  |         31:16        |   15:0   |
// |---------------------------------------------------------------------------------|
// |  reserved |  P |  DPL  | 0 0 1 0 1 | reserved | TSS segment selector | reserved |
// -----------------------------------------------------------------------------------
//
// Interrupt Gate:
// ------------------------------------------------------------------------------------------------
// |      63:48     | 47 | 46:45 |   44:40   | 39:37 |  36:32   |      31:16       |     15:0     |
// |----------------------------------------------------------------------------------------------|
// |  Offset 31..16 |  P |  DPL  | 0 D 1 1 0 | 0 0 0 | reserved | Segment Selector | Offset 15..0 |
// ------------------------------------------------------------------------------------------------
//
// Trap Gate:
// ------------------------------------------------------------------------------------------------
// |      63:48     | 47 | 46:45 |   44:40   | 39:37 |  36:32   |      31:16       |     15:0     |
// |----------------------------------------------------------------------------------------------|
// |  Offset 31..16 |  P |  DPL  | 0 D 1 1 1 | 0 0 0 | reserved | Segment Selector | Offset 15..0 |
// ------------------------------------------------------------------------------------------------
//
// where...
// P -- Present
// D -- Size. This is 1 for a 32-bit gate, or 0 for a 16-bit gate
// DPL -- Descriptor Privilege Level
// Offset -- Offset to procedure entry point
// Segment Selector -- Segment selector for destination code segment
class InterruptDescriptor {
public:
    uint64_t data;

    static constexpr uint64_t SEGMENT_SELECTOR_MASK = 0b0000000000000000000000000000000011111111111111110000000000000000;
    static constexpr uint64_t SEGMENT_SELECTOR_SHIFT = 16;
    static constexpr uint64_t DPL_MASK = 0b0000000000000000011000000000000000000000000000000000000000000000;
    static constexpr uint64_t DPL_SHIFT = 45;
    static constexpr unsigned PRESENT_BIT = 47;
    static constexpr uint64_t OFFSET_LOWER_MASK = 0b00000000000000001111111111111111;
    static constexpr uint64_t OFFSET_UPPER_MASK = 0b11111111111111110000000000000000;
    static constexpr uint64_t OFFSET_UPPER_SHIFT = 48-16;
    

    enum GateType {
        TaskGate,
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
        case TaskGate:
            clear_bit(44);
            clear_bit(43);
            set_bit(42);
            clear_bit(41);
            set_bit(40);
            break;

        case InterruptGate:
            clear_bit(44);
            set_bit(43);
            set_bit(42);
            set_bit(41);
            clear_bit(40);
            clear_bit(39);
            clear_bit(38);
            clear_bit(37);
            break;

        case TrapGate:
            clear_bit(44);
            set_bit(43);
            set_bit(42);
            set_bit(41);
            set_bit(40);
            clear_bit(39);
            clear_bit(38);
            clear_bit(37);
            break;
        }
    }

    GateType get_type() const
    {
        if (get_bit(41) == false) {
            return TaskGate;
        } else if (get_bit(40) == true) {
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

    void set_offset(uint32_t offset)
    {
        data = data & ~(OFFSET_LOWER_MASK);
        data = data | (offset & OFFSET_LOWER_MASK);

        data = data & ~(OFFSET_UPPER_MASK << OFFSET_UPPER_SHIFT);
        data = data | ((offset & OFFSET_UPPER_MASK) << OFFSET_UPPER_SHIFT);
    }

    uint32_t get_offset() const
    {
        return ((data & (OFFSET_UPPER_MASK << OFFSET_UPPER_SHIFT)) >> OFFSET_UPPER_SHIFT) | (data & OFFSET_LOWER_MASK);
    }
};

static_assert(sizeof(InterruptDescriptor) == 8,
              "Intel manual volume 3a, section 6.11 specifies that an IE-32 "
              "interrupt descriptor is exactly eight bytes long.");

} // namespace i386

#endif // FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_INTERRUPT_DESCRIPTOR_HPP
