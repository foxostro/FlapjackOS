#ifndef FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_IDT_HPP
#define FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_IDT_HPP

#include "lidt.hpp"
#include "interrupt_descriptor.hpp"
#include <cstddef>
#include <cstring> // for memset

namespace x86_64 {

// Represents the IE-32e Interrupt Descriptor Table on Intel CPUs.
// For details, refer to the Intel manual, volume 3a, section 6.14.
class InterruptDescriptorTable {
public:
    // Section 6.10 specifies the IDT base pointer should be aligned on an
    // eight byte boundary for best performance. We use 16 here because that is
    // the minimum alignment for the InterruptDescriptor object.
    static constexpr unsigned IDT_BASE_ALIGNMENT = 16;

    // Section 6.10 specifies that an IDT contains entries for at most 256
    // gates.
    static constexpr size_t MAX_ENTRIES = 256;

    alignas(IDT_BASE_ALIGNMENT) InterruptDescriptor entries[MAX_ENTRIES];

    void clear()
    {
        memset(entries, 0, sizeof(entries));
    }

    void load()
    {
        lidt(entries, sizeof(entries)-1);
    }
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_IDT_HPP
