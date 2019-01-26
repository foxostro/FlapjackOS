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

// AFOX_TODO: There's got to be a better way to get a libc/libc++ under test. Maybe I can play game with namespaces?

#ifdef TESTING
#define MALLOC    flapjack_malloc
#define CALLOC    flapjack_calloc
#define MEMALIGN  flapjack_memalign
#define REALLOC   flapjack_realloc
#define FREE      flapjack_free
#define ABORT     flapjack_abort
#else
#define MALLOC    malloc
#define CALLOC    calloc
#define MEMALIGN  memalign
#define REALLOC   realloc
#define FREE      free
#define ABORT     abort
#endif

void* MALLOC(size_t size);
void* CALLOC(size_t count, size_t size);
void* MEMALIGN(size_t size, size_t align);
void* REALLOC(void *ptr, size_t size);
void FREE(void *ptr);
__attribute__((noreturn)) void ABORT();

#ifdef __cplusplus
}
#endif

#endif // FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_STDLIB_H
