#include "kernel.hpp"
#include <type_traits> // for std::aligned_storage
#include <multiboot.h> // AFOX_TODO: REMOVE ME

// Some things need direct access to the kernel object to manipulate the system.
// There is only one kernel. So, it's exposed as a global.
using KernelStorage = typename std::aligned_storage<sizeof(Kernel), alignof(Kernel)>::type;
static KernelStorage g_kernel_storage;
static Kernel* g_kernel_pointer = nullptr;

Kernel& get_global_kernel()
{
    return *g_kernel_pointer;
}

// The kernel must call global constructors itself as we have no runtime
// support beyond what we implement ourselves.
// We want to call this as early as possible after entering kernel_main.
static void call_global_constructors()
{
    for (uintptr_t *ctors_addr = (uintptr_t *)g_start_ctors,
                   *ctors_limit = (uintptr_t *)g_end_ctors;
         ctors_addr < ctors_limit;
         ++ctors_addr) {
        uintptr_t ctor_addr = *ctors_addr;
        using Function = void (*)();
        Function fn = (Function)ctor_addr;
        fn();
    }
}

// This is marked with "C" linkage because we call it from assembly in boot.S.
extern "C" __attribute__((noreturn))
void kernel_main(multiboot_info_t *mb_info, uintptr_t istack)
{
    if (mb_info->mods_count != 1) {
        panic("unexpected number of mods detected: %d", mb_info->mods_count);
    }
    uintptr_t phys = mb_info->mods_addr;
    uintptr_t virt = phys + KERNEL_VIRTUAL_START_ADDR;
    multiboot_module_t* module = reinterpret_cast<multiboot_module_t*>(virt);
    uintptr_t module_address = module->mod_start + KERNEL_VIRTUAL_START_ADDR;
    auto string = reinterpret_cast<char*>(module_address);
    panic("module printed as a string: %s", string);
    
    g_kernel_pointer = (Kernel*)&g_kernel_storage;
    call_global_constructors();
    g_kernel_pointer = new (&g_kernel_storage) Kernel(mb_info, istack);
    get_global_kernel().run();
    g_kernel_pointer->~Kernel();
    panic("We should never reach this point.");
    __builtin_unreachable();
}

// This is marked with "C" linkage because we call it from the assembly code
// ISR stubs in isr_wrapper_asm.S.
extern "C" void interrupt_dispatch_trampoline(void* params)
{
    get_global_kernel().dispatch_interrupt(params);
}

extern "C" void yield()
{
    get_global_kernel().yield();
}

extern "C" __attribute__((noreturn)) void vanish()
{
    get_global_kernel().vanish();
}
