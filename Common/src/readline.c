#include <readline_private.h>
#include <stdbool.h>
#include "flapjack_libc/string.h"
#include "flapjack_libc/assert.h"

#define MIN(a, b) \
   ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
      _a < _b ? _a : _b; })

static void readline_destroy(readline_impl_t *this);
static size_t readline(readline_impl_t *this, size_t buffer_size, char *buffer);

readline_t* readline_init(malloc_interface_t *allocator,
                          console_interface_t *console,
                          keyboard_interface_t *keyboard,
                          size_t prompt_size, const char *prompt)
{
    assert(allocator);
    assert(console);
    assert(keyboard);
    assert(prompt_size>0);
    assert(prompt);

    readline_impl_t *this = allocator->malloc(allocator, sizeof(readline_impl_t));

    this->destroy = readline_destroy;
    this->readline = readline;
    this->allocator = allocator;
    this->console = console;
    this->keyboard = keyboard;
    this->prompt_size = prompt_size;
    this->prompt = prompt;

    return (readline_t *)this;
}

static void readline_destroy(readline_impl_t *this)
{
    assert(this);
    malloc_interface_t *allocator = this->allocator;
    allocator->free(allocator, this);
}

// Prompt for one line of user input on the console.
// Returns the number of characters placed into the buffer.
// buffer_size -- The capacity of the buffer in `buffer'
// buffer -- The output string is written here.
static size_t readline(readline_impl_t *this, size_t buffer_size, char *buffer)
{
    assert(this);
    assert(this->console);
    assert(this->keyboard);
    assert(this->prompt);

    assert(buffer_size > 0);
    assert(buffer);

    bool have_a_newline = false;
    size_t count = 0, cursor_col = 0;
    size_t prompt_len = STRNLEN(this->prompt, this->prompt_size);
    size_t maxcount = MIN(CONSOLE_WIDTH - prompt_len - 1, buffer_size);

    this->console->puts(this->prompt);
    this->console->putchar(' ');
    size_t cursor_row = this->console->get_cursor_row();
    size_t cursor_col_offset = this->console->get_cursor_col();

    while (!have_a_newline) {
        keyboard_event_t event;
        this->keyboard->get_event(&event);

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
                    this->console->set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
                }
                break;

            case KEYCODE_NUMPAD_6:
                // fall through

            case KEYCODE_RIGHT_ARROW:
                if (cursor_col < count) {
                    cursor_col++;
                    this->console->set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
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
                                this->console->draw_char(cursor_row, i + cursor_col_offset,
                                                         this->console->get_char(cursor_row, i + cursor_col_offset + 1));
                            }
                            this->console->draw_char(cursor_row, count + cursor_col_offset, this->console->make_char(' '));
                            buffer[count] = '\0';

                            if (count > 0) {
                                count--;
                            }

                            this->console->set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
                        }
                        break;

                    case '\n':
                        buffer[count] = '\0';
                        cursor_col = count;
                        this->console->set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
                        this->console->putchar('\n');
                        have_a_newline = true;
                        break;

                    default:
                        if ((count < maxcount) && this->console->is_acceptable(ch)) {
                            // Slide all input one character to the right.
                            if (count > 0 && cursor_col < count-1) {
                                for (size_t i = count; i > cursor_col; --i) {
                                    buffer[i] = buffer[i-1];
                                    this->console->draw_char(cursor_row, i + cursor_col_offset,
                                                             this->console->get_char(cursor_row, i + cursor_col_offset - 1));
                                }
                            }
                            this->console->draw_char(cursor_row, cursor_col + cursor_col_offset, this->console->make_char(ch));
                            buffer[cursor_col] = ch;

                            count++;
                            cursor_col++;
                            this->console->set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
                        }
                        break;
                } // switch (ch)
                break;
        } // switch (key)
    } // while

    return count;
}
