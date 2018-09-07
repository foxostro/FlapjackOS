#ifndef FLAPJACKOS_KERNEL_INCLUDE_CLEANUP_KERNEL_MEMORY_MAP_OPERATION_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_CLEANUP_KERNEL_MEMORY_MAP_OPERATION_HPP

#include <cstdint>

// Operation to mark pages read-only which correspond to the kernel image.
// The virtual memory mapping established by the bootstrap start code is simple
// and maps the entire kernel higher-half as read-write.
class CleanupKernelMemoryMapOperation {
public:
    CleanupKernelMemoryMapOperation();
    void cleanup_kernel_memory_map();

private:
    void mark_page_readonly(uintptr_t virtual_address);
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_CLEANUP_KERNEL_MEMORY_MAP_OPERATION_HPP
