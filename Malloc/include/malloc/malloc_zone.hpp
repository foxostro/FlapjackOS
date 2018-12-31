#ifndef FLAPJACKOS_MALLOC_INCLUDE_MALLOC_ZONE_HPP
#define FLAPJACKOS_MALLOC_INCLUDE_MALLOC_ZONE_HPP

#include <cstddef>
#include <cstdint>
#include <common/memory_allocator.hpp>

struct MallocBlock {
    MallocBlock *prev;
    MallocBlock *next;
    size_t size;
    bool inuse;
};

class MallocZone : public MemoryAllocator {
public:
    // Initializes the malloc zone using the specified region of memory.
    // Allocations from the zone will always be taken from this memory region.
    static MallocZone* create(uintptr_t start, size_t size);

    void* malloc(size_t size) override;
    void* memalign(size_t size, size_t align) override;
    void* realloc(void *ptr, size_t new_size) override;
    void free(void *ptr) override;

#ifdef TESTING
    MallocBlock* head() { return head_; };
#endif

private:
    MallocZone();
    MallocBlock* find_best_fit(size_t size);
    MallocBlock* find_best_fit(size_t size, size_t align);

    MallocBlock *head_;
};

#endif // FLAPJACKOS_MALLOC_INCLUDE_MALLOC_ZONE_HPP
