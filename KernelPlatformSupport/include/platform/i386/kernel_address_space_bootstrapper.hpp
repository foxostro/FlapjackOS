#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_KERNEL_ADDRESS_SPACE_BOOTSTRAPPER_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_KERNEL_ADDRESS_SPACE_BOOTSTRAPPER_HPP

#include "paging_resolver.hpp"
#include <platform/i386/generic_kernel_address_space_bootstrapper.hpp>

namespace i386 {

template<typename MMU>
using KernelAddressSpaceBootstrapper = i386::GenericKernelAddressSpaceBootstrapper< i386::PagingResolver<MMU> >;

} // namespace i386

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_KERNEL_ADDRESS_SPACE_BOOTSTRAPPER_HPP