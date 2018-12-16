#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_HARDWARE_TASK_CONFIGURATION_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_HARDWARE_TASK_CONFIGURATION_HPP

#include "gdt.hpp"
#include "tss.hpp"

#include <cstdint>

namespace x86_64 {

class HardwareTaskConfiguration {
public:
    HardwareTaskConfiguration();
    
    // Setup a Task State Segment and Global Descriptor Table for the kernel.
    // The kernel uses one TSS between all tasks and performs software task
    // switching. Also, the kernel uses a flat memory map.
    //
    // This is basic house keeping which must be performed very early in the
    // boot process.
    void init(uint64_t rsp0);

private:
    GlobalDescriptorTable gdt_;
    TaskStateSegment tss_;
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_HARDWARE_TASK_CONFIGURATION_HPP