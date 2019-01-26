#ifdef TESTING
#error "It's not a good idea to try to replace the C++ runtime within the unit test suite. This translation unit is untestable by nature."
#endif

#include <cstdlib>
#include <new>
#include "flapjack_libc/assert.h"
#include <common/logger.hpp>

// The C++ runtime needs this function in case a pure virtual function call is
// made. Though, that should not be possible, anyhow.
extern "C" void __cxa_pure_virtual(void)
{
    panic("__cxa_pure_virtual");
}

void* operator new(size_t size)
{
    return malloc(size);
}

void* operator new[](size_t size)
{
    return malloc(size);
}

void* operator new(size_t size, std::align_val_t align)
{
    return memalign(size, static_cast<size_t>(align));
}

void* operator new[](size_t size, std::align_val_t align)
{
    return memalign(size, static_cast<size_t>(align));
}

void operator delete(void* p)
{
    free(p);
}

void operator delete(void* p, size_t)
{
    free(p);
}

void operator delete(void* p, size_t, std::align_val_t)
{
    free(p);
}

void operator delete[](void* p)
{
    free(p);
}

void operator delete[](void* p, size_t)
{
    free(p);
}

void operator delete[](void* p, size_t, std::align_val_t)
{
    free(p);
}

// C++ runtime support for destructors on globals. TODO: Implement these for real.
void *__dso_handle;
extern "C" int __cxa_atexit(void (*)(void *), void *, void *) { return 0; }
extern "C" void __cxa_finalize(void *) {}
