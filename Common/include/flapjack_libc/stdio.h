#ifndef FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_STDIO_H
#define FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_STDIO_H

#if !defined(__cplusplus) && defined(TESTING)
#error "Use this from C++ only when running under test."
#endif

#ifdef __cplusplus
#include <cstdarg>
#include <cstddef>
#   ifdef TESTING
#   include <cstdio>
#   endif
#else
#include <stdarg.h>
#include <stddef.h>
#   ifdef TESTING
#   include <stdio.h>
#   endif
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

// Similar to traditional vsnprintf.
// Formats the string in manner similar to traditional vsnprintf and returns
// the result in `str'. 
//
// The `size' gives the number of characters that will be placed in `str' minus 
// one. And a terminating null character will be placed at the size-th position.
// This is intended to be the same as traditional vsnprintf.
//
// Supports %d, %u, %c, %s, %x, %X, %p, and %%. Field width specifiers and
// flags are not supported.
//
// Returns the number of characters that would have been output if the size had
// not been limited.
int vsnprintf(char *str,
              size_t size,
              const char *format,
              va_list args);

// Wraps vsnprintf to allow the use of a variable number of arguments.
int snprintf(char *str,
             size_t size,
             const char *format,
             ...);

int vasprintf(char **ret, const char *format, va_list args);
int asprintf(char **ret, const char *format, ...);

int printf(const char *format, ...);

typedef struct {int dummy; } FILE;
extern FILE* stderr;
int fprintf(FILE *stream, const char *format, ...);

int strcmp(const char *s1, const char *s2);

#ifdef __cplusplus
#ifdef TESTING
} // namespace flapjack
#else
} // extern "C"
#endif // TESTING
#endif // __cplusplus

#endif // FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_STDIO_H
