#include "flapjack_libc/string.h"
#include "flapjack_libc/stdlib.h"

extern "C" size_t STRLEN(const char *s)
{
    size_t c = 0;
    if (s) for (; *s; ++c, ++s);
    return c;
}

extern "C" size_t STRNLEN(const char *s, size_t maxlen)
{
    size_t c = 0;
    if (s) for (; c < maxlen && *s; ++c, ++s);
    return c;
}

extern "C" int STRNCMP(const char *s1, const char *s2, size_t n)
{
    while (n && *s1 && (*s1 == *s2)) {
        ++s1;
        ++s2;
        --n;
    }
    if (n == 0) {
        return 0;
    } else {
        return *s1 - *s2;
    }
}

extern "C" char* STRDUP(const char *s)
{
    size_t length = STRLEN(s);
    char* result = (char*)MALLOC(length+1);
    MEMCPY(result, s, length);
    result[length] = 0;
    return result;
}

extern "C" void* MEMCPY(void *dst, const void *src, size_t n)
{
    // We could do better, but this works.

    for (size_t i = 0; i < n; ++i) {
        ((char *)dst)[i] = ((const char *)src)[i];
    }

    return dst;
}

extern "C" void* MEMMOVE(void *dst, const void *src, size_t n)
{
    // We could do better, but this works.

    if (src != dst) {
        size_t begin, end;
        int step;

        if (dst > src) {
            begin = n - 1;
            end = ~0;
            step = -1;
        } else {
            begin = 0;
            end = n;
            step = 1;
        }

        for (size_t i = begin; i != end; i += step) {
            ((char *)dst)[i] = ((const char *)src)[i];
        }
    }

    return dst;
}

#if 0
int memcmp(const void *s1, const void *s2, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        if (s1[i] != s2[i]) {
            return s1[i] - s2[i];
        }
    }

    return 0;
}
#endif

extern "C" void* MEMSET(void *dst, int value, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        ((char *)dst)[i] = (unsigned char)value;
    }

    return dst;
}

int MEMCMP(const void* a, const void* b, size_t n)
{
    const unsigned char *s1 = reinterpret_cast<const unsigned char *>(a);
    const unsigned char *s2 = reinterpret_cast<const unsigned char *>(b);
    while (n--) {
	    unsigned char u1 = *s1;
	    unsigned char u2 = *s2;
	    if (u1 != u2) {
    	    return u1 - u2;
    	}
        s1++;
        s2++;
    }
    return 0;
}
