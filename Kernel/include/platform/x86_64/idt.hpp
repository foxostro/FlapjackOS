#ifndef FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_IDT_HPP
#define FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_IDT_HPP

#include "lidt.hpp"
#include "interrupt_descriptor.hpp"
#include <platform/i386/idt.hpp>

namespace x86_64 {

// Represents the IE-32e Interrupt Descriptor Table on Intel CPUs.
// For details, refer to the Intel manual, volume 3a, section 6.14.
using InterruptDescriptorTable = i386::GenericInterruptDescriptorTable<lidt, InterruptDescriptor>;

} // namespace x86_64

#endif // FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_IDT_HPP
