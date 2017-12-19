#include "flapjack_libc/assert.h"

// The C++ runtime needs this function in case a pure virtual function call is
// made. Though, that should not be possible, anyhow.
extern "C" void __cxa_pure_virtual(void)
{
#ifdef TESTING
    abort();
#else
    panic("__cxa_pure_virtual");
#endif
}
