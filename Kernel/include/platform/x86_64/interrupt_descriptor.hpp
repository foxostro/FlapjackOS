#ifndef FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_INTERRUPT_DESCRIPTOR_HPP
#define FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_INTERRUPT_DESCRIPTOR_HPP

#include <cstdint>
#include <cassert>

namespace x86_64 {

// Represents the IE-32e Interrupt Descriptor on Intel CPUs.
// For details, refer to the Intel manual, volume 3a, section 6.14.
//
// Interrupt/Trap Gate:
// -------------------------------------------------------------------------
// |  127:96  |     95:48      | 47 | 46:45 | 44 |  43:40  | 39:35 | 34:32 |
// |------------------------------------------------------------------------
// | reserved |  Offset 63..16 |  P |  DPL  |  0 |  Type   |   0   |  IST  |
// -------------------------------------------------------------------------
// -----------------------------------
// |      31:16       |     15:0     |
// -----------------------------------
// | Segment Selector | Offset 15..0 |
// -----------------------------------
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
    using uint128 = __uint128_t; // This is a GCC extension, but convenient.
    uint128 data;

    enum GateType {
        InterruptGate,
        TrapGate
    };

    InterruptDescriptor() = default;

    void set_segment_selector(unsigned segment_selector)
    {
        assert(segment_selector < (1<<16));
        // stub
    }

    unsigned get_segment_selector() const
    {
        return 0; // stub
    }

    void set_type([[maybe_unused]] GateType type)
    {
        // stub
    }

    GateType get_type() const
    {
        return InterruptGate; // stub
    }

    void set_bit(unsigned bit_number)
    {
        assert(bit_number < 128);
        uint128 bit = (uint128)1 << bit_number;
        data = data | bit;
    }

    void clear_bit(unsigned bit_number)
    {
        assert(bit_number < 128);
        uint128 bit = (uint128)1 << bit_number;
        data = data & ~bit;
    }

    bool get_bit(unsigned bit_number) const
    {
        assert(bit_number < 128);
        return (data & ((uint128)1 << bit_number)) != 0;
    }

    void set_dpl(unsigned dpl)
    {
        assert(dpl <= 3);
        // stub
    }

    unsigned get_dpl() const
    {
        return 0; // stub
    }

    void set_present([[maybe_unused]] bool present)
    {
        // stub
    }

    bool get_present() const
    {
        return false; // stub
    }

    void set_offset([[maybe_unused]] uint32_t offset)
    {
        // stub
    }

    uint64_t get_offset() const
    {
        return 0; // stub
    }
};

static_assert(sizeof(InterruptDescriptor) == 16,
              "Intel manual volume 3a, section 6.14.1 specifies that an IE-32e "
              "interrupt descriptor is exactly sixteen bytes long.");

} // namespace x86_64

#endif // FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_INTERRUPT_DESCRIPTOR_HPP
