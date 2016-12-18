#include <stdbool.h>
#include <console.h>
#include <keyboard.h>
#include <kprintf.h>
#include <strings.h>
#include <readline.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

size_t readline(const char * const prompt, size_t buffer_size, char *buffer)
{
    bool have_a_newline = false;
    size_t count = 0;
    size_t maxcount = MIN(CONSOLE_WIDTH - strlen(prompt) - 1, buffer_size);

    console_puts(prompt);
    console_putchar(' ');

    while (!have_a_newline) {
        keyboard_event_t event;
        keyboard_get_event(&event);

        if (event.state == PRESSED) {
            char ch = event.ch;

            switch (ch) {
                case '\b':
                    buffer[count] = '\0';
                    if (count > 0) {
                        console_putchar('\b');
                        count--;
                    }
                    break;

                case '\n':
                    buffer[count] = '\0';
                    console_putchar('\n');
                    have_a_newline = true;
                    break;

                default:
                    if ((count < maxcount) && console_is_acceptable(ch)) {
                        console_putchar(ch);
                        buffer[count] = ch;
                        count++;
                        buffer[count] = '\0';
                    }
                    break;
            }
        }
    }

    return count;
}
