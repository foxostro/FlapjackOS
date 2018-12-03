#ifndef FLAPJACKOS_KERNEL_INCLUDE_KERNEL_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_KERNEL_HPP

#include <multiboot.h>
#include <page_frame_allocator.hpp>
#include <interrupt_dispatcher.hpp>
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
    using KernelAddressSpaceBootstrapper = KernelPolicy::KernelAddressSpaceBootstrapper;
    using TextDisplayDevice = KernelPolicy::TextDisplayDevice;

    // Boot the kernel and device drivers.
    // The kernel is the thing that invokes constructors for globals! So, we
    // must do two-phase initialization with a nearly empty constructor and then
    // this method, which performs the real initialization work.
    void init(multiboot_info_t *mb_info, uintptr_t istack);

    // Returns the name of the platform we were built for, e.g., "x86_64".
    const char* get_platform() const;

    // Enter the event loop until the kernel is done. This may never return.
    void run();

    // Redirect the interrupt to the appropriate handler.
    template<typename Params> inline
    void dispatch_interrupt(unsigned interrupt_number,
                            const Params& params) noexcept
    {
        interrupt_dispatcher_.dispatch(interrupt_number, params);
    }

    // Disables interrupts.
    // If interrupts have not been configured and intially enabled then this
    // does nothing.
    void disable_interrupts();

    // Enables interrupts.
    // If interrupts have not been configured and intially enabled then this
    // does nothing.
    void enable_interrupts();

    // TODO: Is this messy?
    HardwareInterruptController& get_hardware_interrupt_controller() noexcept
    {
        return hardware_interrupt_controller_;
    }

private:
    HardwareTaskConfiguration hardware_task_configuration_;
    HardwareInterruptController hardware_interrupt_controller_;
    KeyboardDevice* keyboard_;
    InterruptDispatcher interrupt_dispatcher_;
    TextDisplayDevice display_;
    TextTerminal terminal_;
    multiboot_info_t *mb_info_;
    KernelAddressSpaceBootstrapper address_space_bootstrapper_;
    PageFrameAllocator page_frame_allocator_;
    bool are_interrupts_ready_;

    // The kernel must call global constructors itself as we have no runtime
    // support beyond what we implement ourselves.
    // We want to call this as early as possible after booting. However, we also
    // want the constructors to be able to at least panic() on error. So, call
    // this after initializing interrupts and after initializing the console.
    //
    // This is basic house keeping which must be performed very early in the
    // boot process.
    void call_global_constructors();

    // Invoke the ctor at the specified address.
    void invoke_ctor(uintptr_t ctor_addr);

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

extern Kernel g_kernel;

#endif // FLAPJACKOS_KERNEL_INCLUDE_KERNEL_HPP
