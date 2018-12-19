#include <platform/x86_64/panic_interrupt_handler.hpp>
#include <panic.h>
#include <common/minmax.hpp>
#include <cstring>

namespace x86_64 {

PanicInterruptHandler::PanicInterruptHandler(const char *message, bool error_code_present)
 : error_code_present_(error_code_present)
{
    memset(message_, 0, sizeof(message_));
    memcpy(message_, message, MIN(sizeof(message_), strlen(message))-1);
}

void PanicInterruptHandler::int_handler(const InterruptParameters& params) noexcept
{
    // AFOX_TODO: STUB. Also, panic2() is extremely i386-specific.
    (void)params;
    (void)error_code_present_;
    panic(message_);
}

} // namespace x86_64
