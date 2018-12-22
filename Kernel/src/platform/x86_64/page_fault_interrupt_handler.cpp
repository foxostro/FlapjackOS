#include <platform/x86_64/page_fault_interrupt_handler.hpp>
#include <platform/x86_64/creg.hpp>
#include <cstdio>
#include <panic.h>

namespace x86_64 {

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

PageFaultInterruptHandler::PageFaultInterruptHandler() = default;

void PageFaultInterruptHandler::int_handler(const InterruptParameters& params)
{
    char message[128]={0};
    snprintf(message, sizeof(message),
             "Page Fault.\nfaulting_address: %p\nerror: %s (%u)\n",
              (void*)(uintptr_t)get_cr2(),
              get_page_fault_error_string(params.error_code),
              (unsigned)params.error_code);
    panic(message);
}

} // namespace x86_64
