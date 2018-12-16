#include <page_fault_interrupt_handler.hpp>
#include <cstdio>

// AFOX_TODO: These platform-specific includes seem like a code smell. Perhaps
// the entire page fault interrupt handler should be made to be a
// platform-specific class. This would collect information such as the faulting
// address in a platform-specific manner and would then trampoline to a second,
// platform-agnostic handler.
#ifdef __i386__
#include <platform/i386/creg.hpp>
#elif __x86_64__
#include <platform/x86_64/creg.hpp>
#endif

static uintptr_t get_faulting_address()
{
    #ifdef __i386__
    return i386::get_cr2();
    #elif __x86_64__
    return x86_64::get_cr2();
    #elif __arm__
    #error "interrupt_dispatch is unimplemented for ARM"
    return 0;
    #else
    #error "interrupt_dispatch is unimplemented for this unknown system"
    return 0;
    #endif
}

// Returns a string interpretation of a page fault error code.
static const char* get_page_fault_error_string(unsigned error_code)
{
    switch (error_code) {
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

PageFaultInterruptHandler::PageFaultInterruptHandler() = default;

void PageFaultInterruptHandler::int_handler(const ParameterPackage& params)
{
    uintptr_t faulting_address = get_faulting_address();
    panic_on_unrecoverable_page_fault(faulting_address, params.error_code);
}
