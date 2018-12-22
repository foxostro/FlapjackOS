#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_HARDWARE_INTERRUPT_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_HARDWARE_INTERRUPT_CONTROLLER_HPP

#include <platform/i386/hardware_interrupt_controller.hpp>
#include "idt.hpp"

namespace x86_64 {

struct HardwareInterruptControllerPolicy {
    using OffsetType = uint64_t;
    using InterruptDescriptor = x86_64::InterruptDescriptor;
    using InterruptDescriptorTable = x86_64::InterruptDescriptorTable;
};

using HardwareInterruptController = i386::GenericHardwareInterruptController<HardwareInterruptControllerPolicy>;

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_HARDWARE_INTERRUPT_CONTROLLER_HPP
