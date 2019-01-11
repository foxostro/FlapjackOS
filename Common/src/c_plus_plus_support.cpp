#include "flapjack_libc/assert.h"
#include <cstdlib>
#include <new>

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

#ifdef TESTING
extern "C" __attribute__((noreturn)) void flapjack_abort()
{
    abort();
    __builtin_unreachable();
}
#else
extern "C" __attribute__((noreturn)) void flapjack_abort()
{
    panic("flapjack_abort");
}

extern "C" __attribute__((noreturn)) void abort()
{
    flapjack_abort();
}
#endif

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
