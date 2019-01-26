#include "flapjack_libc/string.h"
#include "flapjack_libc/stdlib.h"

// If we're running under test then insert these symbols into the "flapjack"
// namespace. Otherwise, insert into the global namespace as would be done for
// libc.
#ifdef TESTING
namespace flapjack {
#else
extern "C" {
#endif // TESTING

size_t strlen(const char *s)
{
    size_t c = 0;
    if (s) for (; *s; ++c, ++s);
    return c;
}

size_t strnlen(const char *s, size_t maxlen)
{
    size_t c = 0;
    if (s) for (; c < maxlen && *s; ++c, ++s);
    return c;
}

int strncmp(const char *s1, const char *s2, size_t n)
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

char* strdup(const char *s)
{
    size_t length = strlen(s);
    char* result = (char*)malloc(length+1);
    memcpy(result, s, length);
    result[length] = 0;
    return result;
}

void* memcpy(void *dst, const void *src, size_t n)
{
    // We could do better, but this works.

    for (size_t i = 0; i < n; ++i) {
        ((char *)dst)[i] = ((const char *)src)[i];
    }

    return dst;
}

void* memmove(void *dst, const void *src, size_t n)
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

void* memset(void *dst, int value, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        ((char *)dst)[i] = (unsigned char)value;
    }

    return dst;
}

int memcmp(const void* a, const void* b, size_t n)
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

long strtol(__attribute__((unused)) const char *str, __attribute__((unused)) char **endptr, __attribute__((unused)) int base)
{
    return 0; // stub
}

char* strstr(const char *haystack, const char *needle)
{
    if (!*needle) {
        return (char *)haystack;
    }
    if (!*haystack) {
        return nullptr;
    }
    if ((*haystack == *needle) && (strstr(haystack+1, needle+1) == haystack+1)) {
        return (char *)haystack;
    }
    return strstr(haystack+1, needle);
}

#ifdef TESTING
} // namespace flapjack
#else
} // extern "C"
#endif // TESTING
