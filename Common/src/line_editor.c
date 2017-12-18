#include <line_editor_private.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MIN(a, b) \
   ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
      _a < _b ? _a : _b; })

static void line_editor_destroy(line_editor_impl_t *this)
{
    free(this);
}

// Prompt for one line of user input on the console.
static char * line_editor_getline(line_editor_impl_t *this)
{
    assert(this);
    assert(this->console);
    assert(this->keyboard);
    assert(this->prompt);

    static const size_t buffer_size = 512;
    char *buffer = malloc(buffer_size);

    bool have_a_newline = false;
    size_t count = 0, cursor_col = 0;
    size_t prompt_len = strnlen(this->prompt, this->prompt_size);
    size_t maxcount = MIN(CONSOLE_WIDTH - prompt_len - 1, buffer_size);

    this->console->puts(this->prompt);
    this->console->putchar(' ');
    size_t cursor_row = this->console->get_cursor_row();
    size_t cursor_col_offset = this->console->get_cursor_col();

    while (!have_a_newline) {
        keyboard_event_t event;
        this->keyboard->get_event(this->keyboard, &event);

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

    return buffer;
}

// Change the prompt displayed at the beginning of the line.
static void line_editor_set_prompt(line_editor_impl_t *this,
                                   size_t prompt_size, const char *prompt)
{
    assert(this);
    assert(prompt_size > 0);
    assert(prompt);

    free(this->prompt);

    // Let's always allocate one more byte then requested just to make sure we
    // always have a nul-terminator at the end of the string.
    this->prompt = malloc(prompt_size+1);
    memset(this->prompt, 0, prompt_size+1);
    memcpy(this->prompt, prompt, prompt_size);
    this->prompt_size = prompt_size;
}

// Returns a new initialized line_editor object.
line_editor_t* line_editor_init(console_interface_t *console,
                                keyboard_interface_t *keyboard)
{
    assert(console);
    assert(keyboard);

    line_editor_impl_t *this = malloc(sizeof(line_editor_impl_t));

    this->destroy = line_editor_destroy;
    this->getline = line_editor_getline;
    this->set_prompt = line_editor_set_prompt;
    this->console = console;
    this->keyboard = keyboard;
    this->prompt_size = 0;
    this->prompt = NULL;

    static const char default_prompt[] = ">";
    line_editor_set_prompt(this, sizeof(default_prompt), default_prompt);

    return (line_editor_t *)this;
}