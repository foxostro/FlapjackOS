#include <common/line_editor.hpp>
#include <common/ring_buffer.hpp>
#include <common/text_terminal.hpp>
#include <common/global_allocator.hpp>
#include <malloc/malloc_zone.hpp>

#include <seg.h>
#include <gdt.h>
#include <idt.h>
#include <tss.h>
#include <ltr.h>
#include <interrupt_asm.h>
#include <isr_install.h>
#include <pic.h>
#include <inout.h>
#include <halt.h>
#include <panic.h>
#include <backtrace.hpp>
#include <multiboot.h>
#include <brk.hpp>
#include <creg.h>
#include <kernel_image_info.hpp>

#include <vga_text_display_device.hpp>
#include <pit_timer_device.hpp>
#include <ps2_keyboard_device.hpp>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#define MAGIC_BREAK asm volatile("xchg %bx, %bx")

// We pass the terminal around globally becuase it severely clutters the
// interface of assert() and panic() if we are reqired to pass the terminal
// around literally everywhere.
text_terminal *g_terminal = NULL;

static gdt_entry_t s_gdt[6];
static tss_struct_t s_tss;
static idt_entry_t s_idt[IDT_MAX];
static ps2_keyboard_device *s_keyboard;
static pit_timer_device *s_timer;

// Returns a string interpretation of a page fault error code.
static const char* get_page_fault_error_string(unsigned error_code)
{
    switch (error_code)
    {
        case 0b000: return "supervisory process tried to read a non-present page entry";
        case 0b001: return "supervisory process tried to read a page and caused a protection fault";
        case 0b010: return "supervisory process tried to write to a non-present page entry";
        case 0b011: return "supervisory process tried to write a page and caused a protection fault";
        case 0b100: return "user process tried to read a non-present page entry";
        case 0b101: return "user process tried to read a page and caused a protection fault";
        case 0b110: return "user process tried to write to a non-present page entry";
        case 0b111: return "user process tried to write a page and caused a protection fault";
        default: return "unknown";
    }
}

// Called when a page fault occurs.
static void page_fault_handler(unsigned error_code);

// This is marked with "C" linkage because we call it from the assembly code
// ISR stubs in isr_wrapper_asm.S.
extern "C"
void interrupt_dispatch(unsigned interrupt_number,
                        unsigned edi,
                        unsigned esi,
                        unsigned ebp,
                        unsigned esp,
                        unsigned ebx,
                        unsigned edx,
                        unsigned ecx,
                        unsigned eax,
                        unsigned error_code,
                        unsigned eip)
{
    bool spurious = pic_clear(interrupt_number);

    if (spurious) {
        return;
    }

    switch (interrupt_number) {
        case IDT_KEY:
            s_keyboard->int_handler();
            break;

        case IDT_TIMER:
            s_timer->int_handler();
            break;

        case IDT_DE:
            panic2("Division Error", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_DB:
            panic2("Debug Exception", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_NMI:
            panic2("Non-Maskable Interrupt", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_BP:
            panic2("Breakpoint", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_OF:
            panic2("Overflow", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_BR:
            panic2("BOUND Range exceeded", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_UD:
            panic2("Undefined Opcode", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_NM:
            panic2("No Math coprocessor", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_DF:
            panic2("Double Fault.", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, true, error_code, eip);
            break;

        case IDT_CSO:
            panic2("Coprocessor Segment Overrun", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_TS:
            panic2("Invalid Task Segment Selector.", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, true, error_code, eip);
            break;

        case IDT_NP:
            panic2("Segment Not Present.", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, true, error_code, eip);
            break;

        case IDT_SS:
            panic2("Stack Segment Fault.", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, true, error_code, eip);
            break;

        case IDT_GP:
            panic2("General Protection Fault.", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, true, error_code, eip);
            break;

        case IDT_PF:
            page_fault_handler(error_code);
            break;

        case IDT_MF:
            panic2("X87 Math Fault", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_AC:
            panic2("Alignment Check", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_MC:
            panic2("Machine Check", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_XF:
            panic2("SSE Floating Point Exception", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        default:
            panic("Unknown interrupt: 0x%x", interrupt_number);
    }
}

// The kernel must call global constructors itself as we have no runtime
// support beyond what we implement ourselves.
// We want to call this as early as possible after booting. However, we also
// want the constructors to be able to at least panic() on error. So, call
// this after initializing interrupts and after initializing the console.
static void call_global_constructors()
{
    extern char start_ctors[];
    extern char end_ctors[];

    for (char *ctor = start_ctors;
         ctor < end_ctors;
         ctor += sizeof(void *)) {

        // Read the address from the table, interpret as a function pointer,
        // and then execute the ctor.
        void *value = *((void **)ctor);
        void (*ctor_function_pointer)() = (void (*)())(value);
        ctor_function_pointer();
    }
}

static void invlpg(void* virtual_address)
{
    asm volatile("invlpg (%0)" : : "b"(virtual_address) : "memory");
}

// Convert the given physical address to a logical address.
// These addresses are easily converted between physical and virtual as they are
// always offset by KERNEL_VIRTUAL_START_ADDR.
static uintptr_t convert_physical_to_logical_address(uintptr_t physical_address)
{
    return physical_address + KERNEL_VIRTUAL_START_ADDR;
}

// Convert the given logical address to a physical address.
// These addresses are easily converted between physical and virtual as they are
// always offset by KERNEL_VIRTUAL_START_ADDR.
static uintptr_t convert_logical_to_physical_address(uintptr_t logical_address)
{
    assert(logical_address >= KERNEL_VIRTUAL_START_ADDR);
    return logical_address - KERNEL_VIRTUAL_START_ADDR;
}

// Get the index of the page directory associated with the given address.
static size_t page_directory_index_for_virtual_address(uintptr_t virtual_address)
{
    constexpr uint32_t PDE_INDEX_SHIFT = 22;
    return (uint32_t)virtual_address >> PDE_INDEX_SHIFT;
}

// Get the index of the page table associated with the given address.
static size_t page_table_index_for_virtual_address(uintptr_t virtual_address)
{
    constexpr uint32_t PTE_INDEX_SHIFT = 12;
    constexpr uint32_t PTE_INDEX_MASK = 0x03FF;
    return (uint32_t)virtual_address >> PTE_INDEX_SHIFT & PTE_INDEX_MASK;
}

constexpr uint32_t PHYS_MAP_PRESENT = (1 << 0);
constexpr uint32_t PHYS_MAP_WRITEABLE = (1 << 1);

#define assert_is_page_aligned(address) assert(((address) & (PAGE_SIZE-1)) == 0)

static void map_page(uintptr_t physical_address, uintptr_t virtual_address)
{
    assert_is_page_aligned(physical_address);
    assert_is_page_aligned(virtual_address);
    uint32_t* page_directory = (uint32_t*)convert_physical_to_logical_address(get_cr3());
    size_t page_directory_index = page_directory_index_for_virtual_address(virtual_address);
    size_t page_table_index = page_table_index_for_virtual_address(virtual_address);
    uint32_t& pde = page_directory[page_directory_index];
    pde = pde | PHYS_MAP_WRITEABLE | PHYS_MAP_PRESENT;
    uintptr_t page_table_physical_address = pde & ~(PAGE_SIZE-1);
    uint32_t* page_table = (uint32_t*)convert_physical_to_logical_address(page_table_physical_address);
    uint32_t& pte = page_table[page_table_index];
    pte = physical_address | PHYS_MAP_WRITEABLE | PHYS_MAP_PRESENT;
    invlpg((void*)virtual_address);
}

static uintptr_t get_page_for_address(uintptr_t address)
{
    return address & ~(PAGE_SIZE-1);
}

static __attribute__((noreturn))
void panic_on_unrecoverable_page_fault(uintptr_t faulting_address,
                                       unsigned error_code)
{
    char message[128]={0};
    snprintf(message, sizeof(message),
             "Page Fault.\nfaulting_address: %p\nerror: %s (%u)\n",
              (void*)faulting_address,
              get_page_fault_error_string(error_code),
              error_code);
    panic(message);
}

static bool does_page_fault_error_have_user_bit_set(unsigned error_code)
{
    return (error_code & 0b100) != 0;
}

static bool does_page_fault_error_have_present_bit_set(unsigned error_code)
{
    return (error_code & 0b001) != 0;
}

static bool is_address_in_dynamic_kernel_heap_region(uintptr_t address)
{
    return (address >= (uintptr_t)get_bootstrap_heap_end()) &&
           (address <= (uintptr_t)get_break_pointer());
}

static bool is_page_fault_for_unmapped_kernel_heap(uintptr_t faulting_address,
                                                   unsigned error_code)
{
    return !does_page_fault_error_have_user_bit_set(error_code) &&
           !does_page_fault_error_have_present_bit_set(error_code) &&
           is_address_in_dynamic_kernel_heap_region(faulting_address);
}

static void map_unmapped_kernel_heap_page(uintptr_t faulting_address)
{
    uintptr_t faulting_page = get_page_for_address(faulting_address);
    map_page(convert_logical_to_physical_address(faulting_page),
             faulting_page);
}

static void page_fault_handler(unsigned error_code)
{
    uintptr_t faulting_address = (uintptr_t)get_cr2();
    if (is_page_fault_for_unmapped_kernel_heap(faulting_address, error_code)) {
        map_unmapped_kernel_heap_page(faulting_address);
    } else {
        panic_on_unrecoverable_page_fault(faulting_address, error_code);
    }
}

// Modify the kernel page tables to mark the page associated with the given
// virtual address as readonly.
// The address must be in the kernel memory region.
static void mark_page_readonly(void* virtual_address)
{
    size_t page_directory_index = page_directory_index_for_virtual_address((uintptr_t)virtual_address);
    size_t page_table_index = page_table_index_for_virtual_address((uintptr_t)virtual_address);
    uintptr_t page_table_physical_address = bootstrap_page_directory[page_directory_index] & ~(PAGE_SIZE-1);
    uint32_t* page_table = (uint32_t*)convert_physical_to_logical_address(page_table_physical_address);
    uint32_t& pte = page_table[page_table_index];
    pte = pte & ~PHYS_MAP_WRITEABLE;
    invlpg(virtual_address);
}

// Ensure the text and rodata sections are marked readonly.
// The assembly boot code was sloppy and marked these as writable.
static void cleanup_kernel_memory_map()
{
    for (uintptr_t page = (uintptr_t)kernel_image_begin;
         page < (uintptr_t)kernel_rodata_end;
         page += PAGE_SIZE) {
        mark_page_readonly((void*)page);
    }
}

// Given a memory address, round up to the next page boundary.
static uintptr_t round_address_up_to_next_page(uintptr_t address)
{
    return (address & ~(PAGE_SIZE-1)) + PAGE_SIZE;
}

static void align_break_pointer_on_page_boundary()
{
    brk((void*)round_address_up_to_next_page((uintptr_t)sbrk(0)));
}

// Allocate page tables from kernel heap and use them to populate the kernel
// page directory. The bootstrap page tables have been configured to ensure
// that enough memory is mapped into the kernel virtual memory space to
// provide the 4MB of page tables that we need to do this.
// NOTE: This won't work correctly when using a kernel virtual memory region
// that is larger than 1GB.
static void populate_kernel_page_directory()
{
    constexpr size_t PAGE_TABLE_SIZE = 4096;
    constexpr size_t NUMBER_OF_PAGE_DIRECTORY_ENTRIES = 1024;

    align_break_pointer_on_page_boundary();

    size_t number_of_page_tables = NUMBER_OF_PAGE_DIRECTORY_ENTRIES-NUMBER_OF_BOOTSTRAP_PAGE_TABLES;
    size_t page_table_allocation_size = PAGE_TABLE_SIZE * number_of_page_tables;
    void* page_table_pointer = bootstrap_sbrk(page_table_allocation_size);
    uintptr_t page_table_virtual_address = (uintptr_t)page_table_pointer;
    size_t index_of_first_open_pde = page_directory_index_for_virtual_address(KERNEL_VIRTUAL_START_ADDR + 0x400000*NUMBER_OF_BOOTSTRAP_PAGE_TABLES);

    for (size_t i = index_of_first_open_pde; i < NUMBER_OF_PAGE_DIRECTORY_ENTRIES; ++i) {
        memset((void*)page_table_virtual_address, 0, PAGE_TABLE_SIZE);
        uintptr_t page_table_physical_address = convert_logical_to_physical_address(page_table_virtual_address);
        bootstrap_page_directory[i] = page_table_physical_address | PHYS_MAP_PRESENT;
        page_table_virtual_address += PAGE_TABLE_SIZE;
    }
}

// Enumerate available page frames in the multiboot memory map.
template<typename Function>
static void enumerate_page_frames(multiboot_info_t *mb_info, Function &&fn)
{
    if (!(mb_info->flags & MULTIBOOT_MEMORY_INFO)) {
        panic("The bootloader did not provide memory information.");
    }

    multiboot_memory_map_t *entry = (multiboot_memory_map_t *)convert_physical_to_logical_address(mb_info->mmap_addr);
    multiboot_memory_map_t *limit = entry + mb_info->mmap_length;

    while (entry < limit && entry->size>0) {
        if (entry->type == MULTIBOOT_MEMORY_AVAILABLE) {
            uintptr_t page_frame = entry->base_addr_low;
            for (size_t length = entry->length_low; length>0; length-=PAGE_SIZE, page_frame+=PAGE_SIZE) {
                fn(page_frame);
            }
        }

        entry = (multiboot_memory_map_t*)((uintptr_t)entry + entry->size + sizeof(entry->size));
    }
}

// Return true when the page frame is within the bootstrap kernel memory which
// was mapped in the bootstrap assembly code.
static bool is_page_frame_acceptable(uintptr_t page_frame)
{
    return page_frame >= ((uintptr_t)get_bootstrap_heap_end() - KERNEL_VIRTUAL_START_ADDR);
}

// Initialize the page frame allocator.
// The bootstrap page tables have been configured to ensure that enough memory
// is mapped into the kernel virtual memory space to provide the 4MB for the
// page frame allocator. This is enough for frames when we have 4GB of physical
// memory.
// NOTE: This won't work correctly when we have more thna 4GB of physical memory
// but a solution to that problem would involve either using PAE or a x86_64
// kernel. So, it's an acceptable limitation for now.
constexpr size_t MAX_PAGE_FRAMES = 1048576;
using page_frame_allocator_t = ring_buffer<uintptr_t, MAX_PAGE_FRAMES>;
page_frame_allocator_t* g_page_frame_allocator = NULL;
static void initialize_page_frame_allocator(multiboot_info_t *mb_info)
{
    void *page_frame_allocator_address = bootstrap_sbrk(sizeof(page_frame_allocator_t));
    g_page_frame_allocator = new (page_frame_allocator_address) page_frame_allocator_t;
    enumerate_page_frames(mb_info, [](uintptr_t page_frame){
        if (is_page_frame_acceptable(page_frame)) {
            g_page_frame_allocator->push_back(page_frame);
        }
    });
    g_terminal->printf("Number of free page frames is %u (%uMB)\n", (unsigned)g_page_frame_allocator->size(), (unsigned)g_page_frame_allocator->size()*4/1024);
}

// Initialize the kernel malloc allocator in kernel heap memory.
static memory_allocator* initialize_kernel_malloc()
{
    // The kernel malloc zone will initially consume all remaining bytes in the
    // bootstrap heap. This bootstrap start code has ensured this memory is
    // mapped into the kernel virtual memory space.
    g_terminal->printf("get_available_bootstrap_heap_bytes() --> %u (%uKB)\n",
                       get_available_bootstrap_heap_bytes(),
                       get_available_bootstrap_heap_bytes()/1024);

    constexpr size_t EIGHT_MB = 8 * 1024 * 1024;
    size_t length = get_available_bootstrap_heap_bytes() + EIGHT_MB;
    assert(length > 0);
    g_terminal->printf("Malloc zone size is %u KB.\n", (unsigned)length/1024);
    uintptr_t begin = (uintptr_t)sbrk(length);

    // TODO: We probably want to instead clear the pages as page faults are
    //       serviced. However, if we do it this way then we can exercise the
    //       page fault handler at boot time.
    g_terminal->printf("Clearing the zone.\n");
    constexpr int MAGIC_NUMBER_UNINITIALIZED_HEAP = 0xCD;
    memset((void*)begin, MAGIC_NUMBER_UNINITIALIZED_HEAP, length);

    memory_allocator *alloc = malloc_zone::create(begin, length);
    set_global_allocator(alloc);
    return alloc;
}

// This is marked with "C" linkage because we call it from assembly in boot.S.
extern "C"
__attribute__((noreturn))
void kernel_main(multiboot_info_t *mb_info, uint32_t istack)
{
    // Setup the initial Task State Segment. The kernel uses one TSS between all
    // tasks and performs software task switching.
    memset(&s_tss, 0, sizeof(s_tss));
    s_tss.ss0 = SEGSEL_KERNEL_DS;
    s_tss.esp0 = istack;
    s_tss.iomap = sizeof(s_tss);

    // Setup the Global Descriptor Table. The kernel uses a flat memory map.
    memset(s_gdt, 0, sizeof(s_gdt));
    gdt_create_flat_mapping(s_gdt, sizeof(s_gdt), (uintptr_t)&s_tss);
    lgdt(s_gdt, sizeof(s_gdt) - 1);
    load_task_register();

    // Setup the Interrupt Descriptor Table. This wires various IRQs up to their
    // handler functions.
    isr_install(s_idt);

    // Initialize the PIC chip. It is necessary to remap IRQs before enabling
    // interrupts.
    pic_init();

    // Initialize the VGA text console output driver.
    // The driver lives in the kernel stack since we haven't initialized the
    // kernel heap yet.
    vga_text_display_device vga;
    vga.clear();

    // Initialize the text terminal, built on top of the VGA text display, and
    // make it available to other sub-systems which rely on it globally.
    text_terminal term(vga);
    g_terminal = &term;

    call_global_constructors();
    cleanup_kernel_memory_map();
    populate_kernel_page_directory();
    initialize_page_frame_allocator(mb_info);
    initialize_kernel_malloc();

    // Initialize the real VGA console output driver.
    term.printf("%u KB low memory, %u MB high memory\n",
                mb_info->mem_lower, mb_info->mem_upper/1024);

    // Initialize the PS/2 keyboard driver.
    s_keyboard = new ps2_keyboard_device();

    // Configure the PIT timer chip so that it fires an interrupt every 10ms.
    s_timer = new pit_timer_device(pit_timer_device::TIMER_RATE_10ms,
                                   pit_timer_device::TIMER_LEAP_INTERVAL_10ms,
                                   pit_timer_device::TIMER_LEAP_TICKS_10ms);

    // After this point, interrupts will start firing.
    enable_interrupts();

    // Read lines of user input forever, but don't do anything with them.
    // (This operating system doesn't do much yet.)
    {
        line_editor ed(term, *s_keyboard);
        while (true) {
            auto user_input = ed.getline();
            term.puts("Got: ");
            term.putv(user_input);
            term.puts("\n");
            ed.add_history(std::move(user_input));
        }
    }

    panic("We should never reach this point.");
    __builtin_unreachable();
}
