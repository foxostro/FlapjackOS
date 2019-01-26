#ifdef TESTING
#error "It's not a good idea to try to replace the C++ runtime within the unit test suite. This translation unit is untestable by nature."
#endif

#include <cstdlib>
#include <new>

void* operator new(size_t size, std::align_val_t align)
{
    // AFOX_TODO: Should this be moved to libcxxrt with the other operator news?
    return memalign(size, static_cast<size_t>(align));
}

void* operator new[](size_t size, std::align_val_t align)
{
    // AFOX_TODO: Should this be moved to libcxxrt with the other operator news?
    return memalign(size, static_cast<size_t>(align));
}

void operator delete(void* p, size_t, std::align_val_t)
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
