#ifndef FLAPJACKOS_KERNEL_INCLUDE_KERNEL_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_KERNEL_HPP

#include <multiboot.h>
#include <gdt.h>
#include <idt.h>
#include <tss.h>
#include <ps2_keyboard_device.hpp>
#include <pit_timer_device.hpp>
#include <vga_text_display_device.hpp>
#include <kernel_memory_allocators.hpp>
#include <interrupt_dispatcher.hpp>
#include <mmu.hpp>

#include <cstdint>

// The kernel is the heart of the operating system.
// It manages access to memory and resources on the system.
class Kernel {
public:
    // Boot the kernel and device drivers.
    // The kernel is the thing that invokes constructors for globals! So, we
    // must do two-phase initialization with a nearly empty constructor and then
    // this method, which performs the real initialization work.
    void init(multiboot_info_t *mb_info, uintptr_t istack);

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

private:
    static constexpr size_t NUMBER_OF_PAGE_TABLES = KERNEL_MEMORY_REGION_SIZE / PAGE_SIZE / PageTable::NUMBER_OF_PAGE_TABLE_ENTRIES;
    PageTable page_tables_[NUMBER_OF_PAGE_TABLES];
    GDTEntry gdt_[6];
    TaskStateSegment tss_;
    IDTEntry idt_[IDT_MAX];
    PS2KeyboardDevice* keyboard_;
    KernelMemoryAllocators* allocators_;
    InterruptDispatcher interrupt_dispatcher_;
    VGATextDisplayDevice vga_;
    TextTerminal terminal_;
    bool are_interrupts_ready_;

    // Setup a Task State Segment and Global Descriptor Table for the kernel.
    // The kernel uses one TSS between all tasks and performs software task
    // switching. Also, the kernel uses a flat memory map.
    //
    // This is basic house keeping which must be performed very early in the
    // boot process.
    void initialize_tss_and_gdt(uint32_t istack);

    // The kernel must call global constructors itself as we have no runtime
    // support beyond what we implement ourselves.
    // We want to call this as early as possible after booting. However, we also
    // want the constructors to be able to at least panic() on error. So, call
    // this after initializing interrupts and after initializing the console.
    //
    // This is basic house keeping which must be performed very early in the
    // boot process.
    void call_global_constructors();

    // Setup the VGA console and terminal.
    void setup_terminal();

    // Populate the page directory with page tables.
    void populate_page_directory();

    // The virtual memory map established by the bootstrap code is not
    // sufficient for the kernel's general needs. Fix up permissions, &c.
    void cleanup_kernel_memory_map();

    // Initialize interrupts and device drivers.
    void initialize_interrupts_and_device_drivers();
};

extern Kernel g_kernel;

#endif // FLAPJACKOS_KERNEL_INCLUDE_KERNEL_HPP
