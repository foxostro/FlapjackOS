#pragma once

// Sets the allocator to use for the malloc family of functions
// as well as for new/delete when using C++.
class memory_allocator;
void set_global_allocator(memory_allocator *allocator);
