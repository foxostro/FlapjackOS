#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_KERNEL_ADDRESS_SPACE_BOOTSTRAPPER_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_KERNEL_ADDRESS_SPACE_BOOTSTRAPPER_HPP

#include "paging_resolver.hpp"
#include <page_size.hpp> // for PAGE_SIZE
#include <logical_addressing.hpp> // for KERNEL_MEMORY_REGION_SIZE
#include <platform/i386/generic_kernel_address_space_bootstrapper.hpp>

namespace i386 {

using KernelAddressSpaceBootstrapper = i386::GenericKernelAddressSpaceBootstrapper<i386::PagingResolver>;

} // namespace i386

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_KERNEL_ADDRESS_SPACE_BOOTSTRAPPER_HPP