#include <page_fault_interrupt_handler.hpp>
#include <logical_addressing.hpp>
#include <platform/i386/creg.hpp> // AFOX_TODO: This needs to be platform-agnostic too.
#include <cstdio>

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
    uintptr_t faulting_address = (uintptr_t)i386::get_cr2(); // AFOX_TODO: This needs to be platform-agnostic too.
    panic_on_unrecoverable_page_fault(faulting_address, params.error_code);
}
