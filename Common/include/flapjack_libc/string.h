#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

// To prevent name collisions between symbols here and in the host's C standard
// library, we hide the FlapjackOS symbols behind macros. When building for unit
// tests, we rename the symbols to avoid the collision.
#ifdef TESTING
#define MEMCPY  flapjack_memcpy
#define MEMMOVE flapjack_memmove
#define MEMSET  flapjack_memset
#define STRNLEN flapjack_strnlen
#define STRLEN  flapjack_strlen
#else
#define MEMCPY  memcpy
#define MEMMOVE memmove
#define MEMSET  memset
#define STRNLEN strnlen
#define STRLEN  strlen
#endif

void* MEMCPY(void *dst, const void *src, size_t n);
void* MEMMOVE(void *dst, const void *src, size_t n);
void* MEMSET(void *s, int value, size_t n);

size_t STRNLEN(const char *s, size_t maxlen);
size_t STRLEN(const char *s);

#ifdef __cplusplus
}
#endif