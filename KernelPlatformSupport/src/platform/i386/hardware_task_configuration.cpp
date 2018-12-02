#include <platform/i386/hardware_task_configuration.hpp>
#include <seg.h> // for SEGSEL_KERNEL_DS
#include <cstring> // for memset()
#include <ltr.h>

namespace i386 {

HardwareTaskConfiguration::HardwareTaskConfiguration() = default;

void HardwareTaskConfiguration::init(uint32_t istack)
{
	tss_.clear();
    tss_.ss0 = SEGSEL_KERNEL_DS;
    tss_.esp0 = istack;
    tss_.iomap = sizeof(tss_);

    gdt_.establish_flat_mapping((uintptr_t)&tss_);
    gdt_.load();

    load_task_register();
}

} // namespace i386
