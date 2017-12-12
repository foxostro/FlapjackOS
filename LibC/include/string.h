#pragma once

#include <stddef.h>

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

void* MEMCPY(void * restrict dst, const void * restrict src, size_t n);
void* MEMMOVE(void *dst, const void *src, size_t n);
void* MEMSET(void *s, int value, size_t n);

size_t STRNLEN(const char *s, size_t maxlen);
size_t STRLEN(const char *s);