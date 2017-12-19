#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stddef.h>

#ifdef TESTING
#define VSNPRINTF  flapjack_vsnprintf
#define SNPRINTF   flapjack_snprintf
#else
#define VSNPRINTF  vsnprintf
#define SNPRINTF   snprintf
#endif

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
size_t VSNPRINTF(char *str,
                 size_t size,
                 const char *format,
                 va_list args);

// Wraps vsnprintf to allow the use of a variable number of arguments.
size_t SNPRINTF(char *str,
                size_t size,
                const char *format,
                ...);

#ifdef __cplusplus
}
#endif