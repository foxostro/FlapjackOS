#ifdef TESTING
#error "It's not a good idea to try to replace the C++ runtime within the unit test suite. This translation unit is untestable by nature."
#endif

#include <cstdlib>
#include <new>

void* allocate_memory_for_aligned_new(size_t size, std::align_val_t align)
{
    if (0 == size) {
        size = 1;
    }
    void* allocation = memalign(size, static_cast<size_t>(align));
    while (0 == allocation) {
        auto h = std::get_new_handler();
        if (0 != h) {
            h();
        } else {
            throw std::bad_alloc();
        }
        allocation = memalign(size, static_cast<size_t>(align));
    }
    return allocation;
}

void* operator new(size_t size, std::align_val_t align)
{
    // AFOX_TODO: Should this be moved to libcxxrt with the other operator news?
    return allocate_memory_for_aligned_new(size, align);
}

void* operator new[](size_t size, std::align_val_t align)
{
    // AFOX_TODO: Should this be moved to libcxxrt with the other operator news?
    return allocate_memory_for_aligned_new(size, align);
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
