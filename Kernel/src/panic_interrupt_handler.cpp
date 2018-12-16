#include <panic_interrupt_handler.hpp>
#include <panic.h>
#include <common/minmax.hpp>
#include <cstring>

PanicInterruptHandler::PanicInterruptHandler(const char *message, bool error_code_present)
 : error_code_present_(error_code_present)
{
    memset(message_, 0, sizeof(message_));
    memcpy(message_, message, MIN(sizeof(message_), strlen(message))-1);
}

void PanicInterruptHandler::int_handler(const ParameterPackage& params)
{
    panic2(message_,
           params.edi,
           params.esi,
           params.ebp,
           params.esp,
           params.ebx,
           params.edx,
           params.ecx,
           params.eax,
           error_code_present_,
           params.error_code,
           params.eip);
}
