#include "flapjack_libc/stdlib.h"
#include "flapjack_libc/string.h"
#include "flapjack_libc/assert.h"
#include <common/memory_allocator.hpp>
#include <common/interrupt_lock.hpp>
#include <common/lock_guard.hpp>

static InterruptLock g_malloc_mutex;

// Sets the allocator to use for the malloc family of functions.
static MemoryAllocator *g_allocator = nullptr;
void set_global_allocator(MemoryAllocator *a)
{
    assert(a);
    g_allocator = a;
}

// If we're running under test then insert these symbols into the "flapjack"
// namespace. Otherwise, insert into the global namespace as would be done for
// libc.
#ifdef TESTING
namespace flapjack {
#else
extern "C" {
#endif // TESTING

void* malloc(size_t size)
{
    LockGuard guard{g_malloc_mutex};
    assert(g_allocator && "malloc() called without first specifying a global allocator.");
    return g_allocator->malloc(size);
}

void* calloc(size_t count, size_t size)
{
    LockGuard guard{g_malloc_mutex};
    assert(g_allocator && "calloc() called without first specifying a global allocator.");
    void* result = g_allocator->malloc(count * size);
    memset(result, 0, count * size);
    return result;
}

void* memalign(size_t size, size_t align)
{
    LockGuard guard{g_malloc_mutex};
    assert(g_allocator && "memalign() called without first specifying a global allocator.");
    return g_allocator->memalign(size, align);
}

void* realloc(void *ptr, size_t size)
{
    LockGuard guard{g_malloc_mutex};
    assert(g_allocator && "realloc() called without first specifying a global allocator.");
    return g_allocator->realloc(ptr, size);
}

void free(void *ptr)
{
    LockGuard guard{g_malloc_mutex};
    assert(g_allocator && "free() called without first specifying a global allocator.");
    return g_allocator->free(ptr);
}

__attribute__((noreturn))
void abort()
{
#ifdef TESTING
    ::abort();
#else
    panic("flapjack::abort");
#endif
}

#ifdef TESTING
} // namespace flapjack
#else
} // extern "C"
#endif // TESTING
