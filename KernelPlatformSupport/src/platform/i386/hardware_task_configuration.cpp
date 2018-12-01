#include <platform/i386/hardware_task_configuration.hpp>
#include <seg.h>
#include <ltr.h>
#include <cstring> // for memset()

namespace i386 {

HardwareTaskConfiguration::HardwareTaskConfiguration() = default;

void HardwareTaskConfiguration::init(uint32_t istack)
{
    memset(&tss_, 0, sizeof(tss_));
    tss_.ss0 = SEGSEL_KERNEL_DS;
    tss_.esp0 = istack;
    tss_.iomap = sizeof(tss_);

    memset(gdt_, 0, sizeof(gdt_));
    gdt_create_flat_mapping(gdt_, sizeof(gdt_), (uintptr_t)&tss_);
    lgdt(gdt_, sizeof(gdt_) - 1);

    load_task_register();
}

} // namespace i386
