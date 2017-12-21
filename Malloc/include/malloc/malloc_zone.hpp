#pragma once

#include <cstddef>
#include <cstdint>
#include <common/allocator.hpp>

struct malloc_block {
    malloc_block *prev;
    malloc_block *next;
    size_t size;
    bool inuse;
};

class malloc_zone : public allocator {
public:
    // Initializes the malloc zone using the specified region of memory.
    // Allocations from the zone will always be taken from this memory region.
    static malloc_zone* create(uintptr_t start, size_t size);

    void* malloc(size_t size) override;
    void* realloc(void *ptr, size_t new_size) override;
    void free(void *ptr) override;

#ifdef TESTING
    malloc_block* head() { return _head; };
#endif

private:
    malloc_zone();

    malloc_block *_head;
};
