#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_TSS_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_TSS_HPP

#include <cstdint>
#include <cstring> // for memset()

namespace x86_64 {

// TaskStateSegment must match the layout specified in the Intel Manual,
// volume 3a, section 7.7.
class TaskStateSegment {
public:
    uint32_t reserved0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved1;
    uint64_t ist1;
    uint64_t ist2;
    uint64_t ist3;
    uint64_t ist4;
    uint64_t ist5;
    uint64_t ist6;
    uint64_t ist7;
    uint64_t reserved2;
    uint16_t reserved3;
    uint16_t io_map_base_address;

    void clear()
    {
        memset(this, 0, sizeof(*this));
    }
} __attribute__((packed));

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_TSS_HPP