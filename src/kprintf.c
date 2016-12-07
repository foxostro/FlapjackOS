#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

#define SWAP(x, y) do { __typeof__(x) tmp = x; x = y; y = tmp; } while (0)

static void reverse(char *begin, char *end)
{
    while(end > begin)
    {
        SWAP(*begin, *end);
        begin++;
        end--;
    }
}

static size_t insert_char(char c, char **str, size_t *size)
{
    if (*size > 1) {
        *(*str)++ = c;
        (*size)--;
    }
    return 1;
}

static size_t insert_string(char * restrict value, char **str, size_t *size)
{
    size_t i = 0;

    if (value == NULL) {
        value = "NULL";
    }

    while(*value)
    {
        i += insert_char(*value, str, size);
        value++;
    }

    return i;
}

static size_t insert_int(long long value, unsigned base, char **str, size_t *size)
{
    size_t i = 0;
    const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    bool negative = value < 0;
    char tmp[32] = {0};

    if (negative) {
        value = -value;
        i += insert_char('-', str, size);
    }

    char *begin = tmp, *iter = tmp;

    while (value > 0) {
        *iter++ = digits[value % base];
        value /= base;
    }

    // Above, we inserted digits in reverse order. Here, we put them right.
    reverse(begin, iter - 1);

    i += insert_string(begin, str, size);

    return i;
}

size_t kvsnprintf(char * restrict buf,
                  size_t size,
                  const char * restrict fmt,
                  va_list args)
{
    size_t i = 0;
    char *str = buf;
    bool handlingPercent = false;

    if (buf == NULL || size == 0 || fmt == NULL) {
        return 0;
    }

    while (true)
    {
        char c = *fmt++;

        if (c == '\0') {
            if (size >= 1) {
                *str = '\0';
                size = 0;
            }
            break;
        } else if (!handlingPercent) {
            if (c == '%') {
                handlingPercent = true;
            } else {
                i += insert_char(c, &str, &size);
            }
        } else {
            switch(c) {
                case 'd':
                    i += insert_int(va_arg(args, int), 10, &str, &size);
                    break;

                case 'x':
                    i += insert_int(va_arg(args, int), 16, &str, &size);
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

            handlingPercent = false;
        }
    }

    return i;
}

size_t ksnprintf(char * restrict str,
                 size_t size,
                 const char * restrict fmt,
                 ...)
{
    va_list args;
    va_start(args, fmt);
    int c = kvsnprintf(str, size, fmt, args);
    va_end(args);
    return c;
}
