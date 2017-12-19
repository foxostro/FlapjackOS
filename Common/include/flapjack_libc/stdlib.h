#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <interfaces/malloc_interface.h>

#ifdef TESTING
#define MALLOC  flapjack_malloc
#define REALLOC flapjack_realloc
#define FREE    flapjack_free
#else
#define MALLOC  malloc
#define REALLOC realloc
#define FREE    free
#endif

void* MALLOC(size_t size);
void* REALLOC(void *ptr, size_t size);
void FREE(void *ptr);

// Sets the allocator to use for the malloc family of functions.
void set_global_allocator(malloc_interface_t *allocator);

#ifdef __cplusplus
}
#endif