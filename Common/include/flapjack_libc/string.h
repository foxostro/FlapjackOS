#ifndef FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_STRING_H
#define FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_STRING_H

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

void* memcpy(void *dst, const void *src, size_t n);
void* memmove(void *dst, const void *src, size_t n);
void* memset(void *s, int value, size_t n);
int memcmp(const void* s1, const void* s2, size_t n);

size_t strnlen(const char *s, size_t maxlen);
size_t strlen(const char *s);
int strncmp(const char *s1, const char *s2, size_t n);
char* strdup(const char *s);
long strtol(const char *str, char **endptr, int base);
char* strstr(const char *haystack, const char *needle);

#ifdef __cplusplus
#ifdef TESTING
} // namespace flapjack
#else
} // extern "C"
#endif // TESTING
#endif // __cplusplus

#endif // FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_STRING_H
