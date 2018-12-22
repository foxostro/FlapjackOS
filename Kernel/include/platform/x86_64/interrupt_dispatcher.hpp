#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_INTERRUPT_DISPATCHER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_INTERRUPT_DISPATCHER_HPP

#include "hardware_interrupt_controller.hpp"
#include "interrupt_parameters.hpp"
#include <platform/i386/interrupt_dispatcher.hpp>

namespace x86_64 {

using InterruptDispatcher = GenericInterruptDispatcher<x86_64::HardwareInterruptController,
                                                       x86_64::InterruptParameters,
                                                       i386::InterruptNamer>;

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_INTERRUPT_DISPATCHER_HPP
