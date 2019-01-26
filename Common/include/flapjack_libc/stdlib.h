#ifndef FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_STDLIB_H
#define FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_STDLIB_H

#if !defined(__cplusplus) && defined(TESTING)
#error "Use this from C++ only when running under test."
#endif

#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif

// If we're running under test then insert these symbols into the "flapjack"
// namespace. Otherwise, insert into the global namespace as would be done for
// libc.
#ifdef __cplusplus
#ifdef TESTING
namespace flapjack {
#else
extern "C" {
#endif // TESTING
#endif // __cplusplus

void* malloc(size_t size);
void* calloc(size_t count, size_t size);
void* memalign(size_t size, size_t align);
void* realloc(void *ptr, size_t size);
void free(void *ptr);
__attribute__((noreturn)) void abort();

#ifdef __cplusplus
#ifdef TESTING
} // namespace flapjack
#else
} // extern "C"
#endif // TESTING
#endif // __cplusplus

#endif // FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_STDLIB_H
