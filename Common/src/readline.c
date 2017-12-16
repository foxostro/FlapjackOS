#include <stdbool.h>
#include <readline.h>
#include "flapjack_libc/string.h"
#include "flapjack_libc/assert.h"

#define MIN(a, b) \
   ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
      _a < _b ? _a : _b; })

size_t readline(const console_interface_t *console,
                const keyboard_interface_t *keyboard,
                size_t prompt_size, const char * const prompt,
                size_t buffer_size, char *buffer)
{
    bool have_a_newline = false;
    size_t count = 0, cursor_col = 0;
    size_t maxcount = MIN(CONSOLE_WIDTH - STRNLEN(prompt, prompt_size) - 1, buffer_size);

    assert(console);
    assert(keyboard);
    assert(prompt);
    assert(buffer);

    console->puts(prompt);
    console->putchar(' ');
    size_t cursor_row = console->get_cursor_row();
    size_t cursor_col_offset = console->get_cursor_col();

    while (!have_a_newline) {
        keyboard_event_t event;
        keyboard->get_event(&event);

        if (event.state != PRESSED) {
            continue;
        }

        char ch = event.ch;
        keycode_t key = event.key;

        switch (key) {
            case KEYCODE_NUMPAD_4:
                // fall through

            case KEYCODE_LEFT_ARROW:
                if (cursor_col > 0) {
                    cursor_col--;
                    console->set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
                }
                break;

            case KEYCODE_NUMPAD_6:
                // fall through

            case KEYCODE_RIGHT_ARROW:
                if (cursor_col < count) {
                    cursor_col++;
                    console->set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
                }
                break;

            default:
                switch (ch) {
                    case '\b':
                        if (cursor_col > 0) {
                            cursor_col--;

                            // Slide all input one character to the left.
                            for (size_t i = cursor_col; i < count; ++i) {
                                buffer[i] = buffer[i+1];
                                console->draw_char(cursor_row, i + cursor_col_offset,
                                                   console->get_char(cursor_row, i + cursor_col_offset + 1));
                            }
                            console->draw_char(cursor_row, count + cursor_col_offset, console->make_char(' '));
                            buffer[count] = '\0';

                            if (count > 0) {
                                count--;
                            }

                            console->set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
                        }
                        break;

                    case '\n':
                        buffer[count] = '\0';
                        cursor_col = count;
                        console->set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
                        console->putchar('\n');
                        have_a_newline = true;
                        break;

                    default:
                        if ((count < maxcount) && console->is_acceptable(ch)) {
                            // Slide all input one character to the right.
                            if (count > 0 && cursor_col < count-1) {
                                for (size_t i = count; i > cursor_col; --i) {
                                    buffer[i] = buffer[i-1];
                                    console->draw_char(cursor_row, i + cursor_col_offset,
                                                       console->get_char(cursor_row, i + cursor_col_offset - 1));
                                }
                            }
                            console->draw_char(cursor_row, cursor_col + cursor_col_offset, console->make_char(ch));
                            buffer[cursor_col] = ch;

                            count++;
                            cursor_col++;
                            console->set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
                        }
                        break;
                } // switch (ch)
                break;
        } // switch (key)
    } // while

    return count;
}
