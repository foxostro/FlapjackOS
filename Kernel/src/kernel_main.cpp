#include "kernel.hpp"
#include <type_traits> // for std::aligned_storage

// Some things need direct access to the kernel object to manipulate the system.
// There is only one kernel. So, it's exposed as a global.
using KernelStorage = typename std::aligned_storage<sizeof(Kernel), alignof(Kernel)>::type;
static KernelStorage g_kernel_storage;
static Kernel* g_kernel_pointer = nullptr;

Kernel& get_global_kernel()
{
    return *g_kernel_pointer;
}


// AFOX_TODO: Maybe move all this runtime BS out of kernel_main.cpp and into separate source files like c_plus_plus_support.cpp.
static const char __EH_FRAME_BEGIN__[]
__attribute__((section(".eh_frame"), aligned(4)))
= {};

static const int32_t __FRAME_END__[]
__attribute__((used, section(".eh_frame"), aligned(alignof(int32_t)))) = {0};


// Provided by libgcc. Normally called in code in crt0.
struct object;
extern "C" {
extern void __register_frame_info(const void *, struct object *);
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
void kernel_main(multiboot_info_t* mb_info, uintptr_t istack)
{
    static uint32_t buffer[16]; // Must be large enough to hold the object libgcc is expecting.
    __register_frame_info(__EH_FRAME_BEGIN__, (object*)buffer);

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
