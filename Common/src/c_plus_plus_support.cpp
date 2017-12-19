#include "flapjack_libc/assert.h"
#include <cstdlib>

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

void *operator new(size_t size)
{
    return malloc(size);
}

void *operator new[](size_t size)
{
    return malloc(size);
}

void operator delete(void *p)
{
    free(p);
}

void operator delete[](void *p)
{
    free(p);
}
