#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_KERNEL_POLICY_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_KERNEL_POLICY_HPP

#if defined(__x86_64__)

// This is also the arch for the unit test host system.
// TODO: Write platform-specific MMU classes for x86_64.
#include <platform/i386/kernel_policy.hpp>
using KernelPolicy = i386::KernelPolicy;

#elif defined(__i386__)

#include <platform/i386/kernel_policy.hpp>
using KernelPolicy = i386::KernelPolicy;

#elif defined(__arm__)

#error "The ARM-specific MMU classes have not been written yet."

#else

#error "unknown platform"

#endif

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_KERNEL_POLICY_HPP