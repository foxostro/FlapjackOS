#ifndef FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_STRING_H
#define FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_STRING_H

#ifdef __cplusplus

#include <cstddef>
extern "C" {

#else

#include <stddef.h>

#endif


// To prevent name collisions between symbols here and in the host's C standard
// library, we hide the FlapjackOS symbols behind macros. When building for unit
// tests, we rename the symbols to avoid the collision.
#ifdef TESTING
#define MEMCPY  flapjack_memcpy
#define MEMMOVE flapjack_memmove
#define MEMSET  flapjack_memset
#define STRNLEN flapjack_strnlen
#define STRLEN  flapjack_strlen
#define STRNCMP flapjack_strncmp
#else
#define MEMCPY  memcpy
#define MEMMOVE memmove
#define MEMSET  memset
#define STRNLEN strnlen
#define STRLEN  strlen
#define STRNCMP strncmp
#endif

void* MEMCPY(void *dst, const void *src, size_t n);
void* MEMMOVE(void *dst, const void *src, size_t n);
void* MEMSET(void *s, int value, size_t n);

size_t STRNLEN(const char *s, size_t maxlen);
size_t STRLEN(const char *s);

int STRNCMP(const char *s1, const char *s2, size_t n);


#ifdef __cplusplus
}
#endif

#endif // FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_STRING_H
