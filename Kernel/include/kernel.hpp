#ifndef FLAPJACKOS_KERNEL_INCLUDE_KERNEL_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_KERNEL_HPP

#include <multiboot.h>
#include <page_frame_allocator.hpp>
#include <kernel_policy.hpp>
#include <scheduler.hpp>
#include <common/text_terminal.hpp>
#include <cstdint>

// The kernel is the heart of the operating system.
// It manages access to memory and resources on the system.
class Kernel : private KernelPolicy {
public:
    ~Kernel() = default;

    // Boots the kernel and intializes device drivers.
    Kernel(multiboot_info_t *mb_info, uintptr_t istack);

    // Returns the name of the platform we were built for, e.g., "x86_64".
    const char* get_platform() const;

    // Enter the event loop until the kernel is done. This may never return.
    void run();

    // Redirect the interrupt to the appropriate handler.
    void dispatch_interrupt(void* params)
    {
        interrupt_controller_.dispatch(params);
    }

    // The current thread yields its time slice to the next one.
    void yield()
    {
        scheduler_.yield();
    }

private:
    multiboot_info_t *mb_info_;
    uintptr_t istack_;
    HardwareTaskConfiguration hardware_task_configuration_;
    InterruptController interrupt_controller_;
    DeviceDrivers device_drivers_;
    TextDisplayDevice display_;
    TextTerminal terminal_;
    HardwareMemoryManagementUnit mmu_;
    KernelAddressSpaceBootstrapper address_space_bootstrapper_;
    PhysicalMemoryMap phys_map_;
    PageFrameAllocator page_frame_allocator_;
    Scheduler scheduler_;

    // Setup the VGA console and terminal.
    void setup_terminal();

    // Print a welcome message to the terminal.
    void print_welcome_message();

    // The virtual memory map established by the bootstrap code is only
    // sufficient for getting the Kernel to start running. We need to
    // map the rest of the kernel virtual memory region, fix up access
    // flags for the kernel image, and so on.
    void prepare_kernel_address_space();

    // Report to the console the amount of installed system memory.
    void report_installed_memory();

    // Report to the console the number of free page frames.
    void report_free_page_frames();

    // Initialize the page frame allocator according to the multiboot mmap.
    void initialize_page_frame_allocator();

    // Initialize the kernel malloc zone allocator.
    void initialize_kernel_malloc();

    // Initialize interrupts and device drivers.
    void initialize_interrupts_and_device_drivers();
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_KERNEL_HPP
