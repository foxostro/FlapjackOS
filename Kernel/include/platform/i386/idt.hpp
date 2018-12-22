#ifndef FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_IDT_HPP
#define FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_IDT_HPP

#include "lidt.hpp"
#include "interrupt_descriptor.hpp"
#include <cstddef>
#include <cstring> // for memset

namespace i386 {

template<void (*lidt)(void*,unsigned), typename Entry>
class GenericInterruptDescriptorTable {
public:
    // Section 6.10 specifies that an IDT contains entries for at most 256
    // gates.
    static constexpr size_t MAX_ENTRIES = 256;

    Entry entries[MAX_ENTRIES];

    void clear()
    {
        memset(entries, 0, sizeof(entries));
    }

    void load()
    {
        lidt(entries, sizeof(entries)-1);
    }
};

// Represents the IE-32 Interrupt Descriptor Table on Intel CPUs.
// For details, refer to the Intel manual, volume 3a, section 6.10.
using InterruptDescriptorTable = GenericInterruptDescriptorTable<lidt, InterruptDescriptor>;

} // namespace i386

#endif // FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_IDT_HPP
