#include <platform/x86_64/hardware_task_configuration.hpp>
#include <seg.h> // for SEGSEL_KERNEL_DS
#include <cstring> // for memset()
#include <ltr.h>

namespace x86_64 {

HardwareTaskConfiguration::HardwareTaskConfiguration() = default;

void HardwareTaskConfiguration::init(uint64_t rsp0)
{
	tss_.clear();
    tss_.rsp0 = rsp0;
    tss_.io_map_base_address = sizeof(tss_);

    gdt_.init(&tss_);
    gdt_.load();

    load_task_register();
}

} // namespace x86_64
