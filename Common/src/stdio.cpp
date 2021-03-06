#include "flapjack_libc/stdio.h"
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <utility>

#include <common/logger.hpp>

// If we're running under test then insert these symbols into the "flapjack"
// namespace. Otherwise, insert into the global namespace as would be done for
// libc.
#ifdef TESTING
namespace flapjack {
#else
extern "C" {
#endif // TESTING

FILE* stderr = nullptr;

static const char digits_lower[] = "0123456789abcdefghijklmnopqrstuvwxyz";
static const char digits_upper[] = "0123456789ABCDEFGHIJKLOMNPQRSTUVWXYZ";

static void reverse(char *begin, char *end)
{
    while (end > begin) {
        std::swap(*begin, *end);
        begin++;
        end--;
    }
}

static size_t insert_char(char c, char **str, size_t *size)
{
    if (*size > 1) {
        if (isprint(c) || (c == '\n') || (c == '\t')) {
            *(*str)++ = c;
            (*size)--;
        }
    }
    return 1;
}

static size_t insert_string(const char *value, char **str, size_t *size)
{
    size_t i = 0;

    if (value == nullptr) {
        value = "(null)";
    }

    while (*value) {
        i += insert_char(*value, str, size);
        value++;
    }

    return i;
}

static size_t insert_int(int value, unsigned base, const char *digits, char **str, size_t *size)
{
    size_t i = 0;
    bool negative = value < 0;
    char tmp[32] = {0};

    if (negative) {
        value = -value;
        i += insert_char('-', str, size);
    }

    char *begin = tmp, *iter = tmp;

    if (value == 0) {
        *iter++ = digits[0];
    } else while (value > 0) {
        *iter++ = digits[value % base];
        value /= base;
    }

    // Above, we inserted digits in reverse order. Here, we put them right.
    reverse(begin, iter - 1);

    i += insert_string(begin, str, size);

    return i;
}

static size_t insert_uint(unsigned value, unsigned base, const char *digits, char **str, size_t *size)
{
    char tmp[32] = {0};

    char *begin = tmp, *iter = tmp;

    if (value == 0) {
        *iter++ = digits[0];
    } else while (value > 0) {
        *iter++ = digits[value % base];
        value /= base;
    }

    // Above, we inserted digits in reverse order. Here, we put them right.
    reverse(begin, iter - 1);

    size_t i = insert_string(begin, str, size);

    return i;
}

static size_t insert_pointer(uintptr_t value, char **str, size_t *size)
{
    const unsigned base = 16;
    char tmp[32] = {0};
    int c = sizeof(uintptr_t) * 2;
    size_t i = insert_string("0x", str, size);
    char *begin = tmp, *iter = tmp;

    while (value > 0) {
        *iter++ = digits_lower[value % base];
        value /= base;
        c--;
    }

    while (c > 0) {
        if (*size > 1) {
            *iter++ = '0';
            (*size)--;
        }
        c--;
    }

    // Above, we inserted digits in reverse order. Here, we put them right.
    reverse(begin, iter - 1);

    i += insert_string(begin, str, size);

    return i;
}

int vsnprintf(char *buf,
              size_t size,
              const char *fmt,
              va_list args)
{
    size_t i = 0;
    char *str = buf;
    bool handling_percent = false;

    if (buf == nullptr || size == 0 || fmt == nullptr) {
        return 0;
    }

    while (true) {
        char c = *fmt++;

        if (c == '\0') {
            if (size >= 1) {
                *str = '\0';
                size = 0;
            }
            break;
        } else if (!handling_percent) {
            if (c == '%') {
                handling_percent = true;
            } else {
                i += insert_char(c, &str, &size);
            }
        } else {
            switch(c) {
                case 'd':
                    i += insert_int(va_arg(args, int), 10, digits_lower, &str, &size);
                    break;

                case 'u':
                    i += insert_uint(va_arg(args, unsigned), 10, digits_lower, &str, &size);
                    break;

                case 'x':
                    i += insert_uint(va_arg(args, unsigned), 16, digits_lower, &str, &size);
                    break;

                case 'X':
                    i += insert_uint(va_arg(args, unsigned), 16, digits_upper, &str, &size);
                    break;

                case 'p':
                    i += insert_pointer((uintptr_t)va_arg(args, uintptr_t), &str, &size);
                    break;

                case 's':
                    i += insert_string((char *)va_arg(args, char *), &str, &size);
                    break;

                case 'c':
                    i += insert_char((char)va_arg(args, int), &str, &size);
                    break;

                default:
                    i += insert_char(c, &str, &size);
                    break;
            }

            handling_percent = false;
        }
    }

    return i;
}

int snprintf(char *str, size_t size, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int c = vsnprintf(str, size, fmt, args);
    va_end(args);
    return c;
}

int vasprintf(char **ret, const char *fmt, va_list args)
{
    char small[1] = {0};
    int len = vsnprintf(small, sizeof(small), fmt, args);
    char* result = (char*)calloc(len+1, 1);
    int c = vsnprintf(result, len, fmt, args);
    *ret = result;
    return c;
}

int asprintf(char **ret, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int c = vasprintf(ret, fmt, args);
    va_end(args);
    return c;
}

int printf(const char *fmt, ...)
{
    char buffer[128] = {0};
    va_list args;
    va_start(args, fmt);
    int c = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    TRACE(buffer);
    return c;
}

int fprintf(__attribute__((unused)) FILE *stream, const char *fmt, ...)
{
    char buffer[128] = {0};
    va_list args;
    va_start(args, fmt);
    int c = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    TRACE(buffer);
    return c;
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

#ifdef TESTING
} // namespace flapjack
#else
} // extern "C"
#endif // TESTING
