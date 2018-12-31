#ifndef FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_STDLIB_H
#define FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_STDLIB_H

#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TESTING
#define MALLOC  flapjack_malloc
#define MEMALIGN  flapjack_memalign
#define REALLOC flapjack_realloc
#define FREE    flapjack_free
#else
#define MALLOC  malloc
#define MEMALIGN  memalign
#define REALLOC realloc
#define FREE    free
#endif

void* MALLOC(size_t size);
void* MEMALIGN(size_t size, size_t align);
void* REALLOC(void *ptr, size_t size);
void FREE(void *ptr);

#ifdef __cplusplus
}
#endif

#endif // FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_STDLIB_H
