#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_KERNEL_ADDRESS_SPACE_BOOTSTRAPPER_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_KERNEL_ADDRESS_SPACE_BOOTSTRAPPER_HPP

#include "paging_resolver.hpp"
#include <platform/i386/generic_kernel_address_space_bootstrapper.hpp>

namespace x86_64 {

template<typename MMU>
using KernelAddressSpaceBootstrapper = i386::GenericKernelAddressSpaceBootstrapper< x86_64::PagingResolver<MMU> >;

} // namespace x86_64

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_KERNEL_ADDRESS_SPACE_BOOTSTRAPPER_HPP