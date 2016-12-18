#pragma once

#include <stddef.h>

void bzero(void *s, size_t n);
size_t strnlen(const char *s, size_t maxlen);
size_t strlen(const char *s);
void* memcpy(void * restrict dst, const void * restrict src, size_t n);
