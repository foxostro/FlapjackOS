#include <platform/i386/hardware_task_configuration.hpp>
#include <platform/i386/ltr.hpp>
#include <seg.h> // for SEGSEL_KERNEL_DS

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

    ltr();
}

} // namespace i386
