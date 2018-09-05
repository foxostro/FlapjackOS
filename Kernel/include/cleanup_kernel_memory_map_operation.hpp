#pragma once

#include <cstdint>

// Operation to mark pages read-only which correspond to the kernel image.
// The virtual memory mapping established by the bootstrap start code is simple
// and maps the entire kernel higher-half as read-write.
class cleanup_kernel_memory_map_operation {
public:
    cleanup_kernel_memory_map_operation();
    void cleanup_kernel_memory_map();

private:
    void mark_page_readonly(uintptr_t virtual_address);
};
