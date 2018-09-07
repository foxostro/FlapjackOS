#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_GLOBAL_ALLOCATOR_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_GLOBAL_ALLOCATOR_HPP

#include <common/memory_allocator.hpp>

// Sets the allocator to use for the malloc family of functions
// as well as for new/delete when using C++.
void set_global_allocator(MemoryAllocator *allocator);

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_GLOBAL_ALLOCATOR_HPP
