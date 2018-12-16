#ifndef FLAPJACKOS_KERNEL_INCLUDE_KERNEL_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_KERNEL_HPP

#include <multiboot.h>
#include <page_frame_allocator.hpp>
#include <kernel_policy.hpp>

#include <common/keyboard_device.hpp>
#include <common/text_terminal.hpp>

#include <cstdint>

// The kernel is the heart of the operating system.
// It manages access to memory and resources on the system.
class Kernel {
public:
    using HardwareTaskConfiguration = KernelPolicy::HardwareTaskConfiguration;
    using HardwareInterruptController = KernelPolicy::HardwareInterruptController;
    using InterruptDispatcher = KernelPolicy::InterruptDispatcher;
    using HardwareMemoryManagementUnit = KernelPolicy::HardwareMemoryManagementUnit;
    using KernelAddressSpaceBootstrapper = KernelPolicy::KernelAddressSpaceBootstrapper;
    using PhysicalMemoryMap = KernelPolicy::PhysicalMemoryMap;
    using TextDisplayDevice = KernelPolicy::TextDisplayDevice;

    ~Kernel() = default;

    // Boots the kernel and intializes device drivers.
    Kernel(multiboot_info_t *mb_info, uintptr_t istack);

    // Returns the name of the platform we were built for, e.g., "x86_64".
    const char* get_platform() const;

    // Enter the event loop until the kernel is done. This may never return.
    void run();

    // Redirect the interrupt to the appropriate handler.
    void dispatch_interrupt(void* params_raw) noexcept
    {
        InterruptDispatcher::Params* params = (InterruptDispatcher::Params*)params_raw;
        interrupt_dispatcher_.dispatch(*params);
    }

    // Disables interrupts.
    // If interrupts have not been configured and intially enabled then this
    // does nothing.
    void disable_interrupts();

    // Enables interrupts.
    // If interrupts have not been configured and intially enabled then this
    // does nothing.
    void enable_interrupts();

private:
    multiboot_info_t *mb_info_;
    uintptr_t istack_;
    HardwareTaskConfiguration hardware_task_configuration_;
    HardwareInterruptController hardware_interrupt_controller_;
    KeyboardDevice* keyboard_;
    InterruptDispatcher interrupt_dispatcher_;
    TextDisplayDevice display_;
    TextTerminal terminal_;
    HardwareMemoryManagementUnit mmu_;
    KernelAddressSpaceBootstrapper address_space_bootstrapper_;
    PhysicalMemoryMap phys_map_;
    PageFrameAllocator page_frame_allocator_;
    bool are_interrupts_ready_;

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

Kernel& get_global_kernel();

#endif // FLAPJACKOS_KERNEL_INCLUDE_KERNEL_HPP
