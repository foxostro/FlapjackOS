#ifndef FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_STDIO_H
#define FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_STDIO_H

#ifdef __cplusplus
#include <cstdarg>
#include <cstddef>

extern "C" {

#else

#include <stdarg.h>
#include <stddef.h>

#endif

#ifdef TESTING
#define VSNPRINTF  flapjack_vsnprintf
#define SNPRINTF   flapjack_snprintf
#define VASPRINTF  flapjack_vasprintf
#define ASPRINTF   flapjack_asprintf
#define PRINTF     flapjack_printf
#define FPRINTF    flapjack_fprintf
#define STRCMP     flapjack_strcmp
#else
#define VSNPRINTF  vsnprintf
#define SNPRINTF   snprintf
#define VASPRINTF  vasprintf
#define ASPRINTF   asprintf
#define PRINTF     printf
#define FPRINTF    fprintf
#define STRCMP     strcmp
#endif

// Avoid colliding with FILE-related symbols when building for testing.
#ifdef TESTING
#include <stdio.h>
#else
typedef struct {int dummy; } FILE;
extern FILE* stderr;
#endif
#define FLAPJACK_FILE FILE
#define FLAPJACK_STDERR stderr

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
int VSNPRINTF(char *str,
              size_t size,
              const char *format,
              va_list args);

// Wraps vsnprintf to allow the use of a variable number of arguments.
int SNPRINTF(char *str,
             size_t size,
             const char *format,
             ...);

int VASPRINTF(char **ret, const char *format, va_list args);
int ASPRINTF(char **ret, const char *format, ...);

int PRINTF(const char *format, ...);
int FPRINTF(FLAPJACK_FILE *stream, const char *format, ...);

int STRCMP(const char *s1, const char *s2);

#ifdef __cplusplus
}
#endif

#endif // FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_STDIO_H
