#pragma once

#include <stddef.h>
#include <interfaces/malloc_interface.h>

// Initializes the malloc zone using the specified region of memory.
// Returns the address of the malloc zone structure stored within that
// memory.
// Allocations from the zone will always be taken from this memory region.
malloc_interface_t* malloc_zone_init(void *start, size_t size);
