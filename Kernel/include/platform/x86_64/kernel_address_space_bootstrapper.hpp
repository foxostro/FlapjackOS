#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_KERNEL_ADDRESS_SPACE_BOOTSTRAPPER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_KERNEL_ADDRESS_SPACE_BOOTSTRAPPER_HPP

#include "paging_resolver.hpp"
#include <platform/i386/generic_kernel_address_space_bootstrapper.hpp>

namespace x86_64 {

using KernelAddressSpaceBootstrapper = i386::GenericKernelAddressSpaceBootstrapper<x86_64::PagingResolver>;

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_KERNEL_ADDRESS_SPACE_BOOTSTRAPPER_HPP