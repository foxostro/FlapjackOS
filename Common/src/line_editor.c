#include <line_editor_private.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
// #include <console_printf.h>

#define MIN(a, b) \
   ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
      _a < _b ? _a : _b; })

#define MAX(a, b) \
   ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
      _a > _b ? _a : _b; })

static void line_editor_destroy(line_editor_impl_t *this)
{
    while (ll_count_str(this->history) > 0) {
        char *history = ll_remove_str(this->history, 0);
        free(history);
    }
    ll_destroy_str(this->history);

    free(this->prompt);

    free(this);
}

static void backspace(console_interface_t *console,
                      char *buffer,
                      size_t *count,
                      size_t *cursor_col,
                      size_t cursor_row,
                      size_t cursor_col_offset)
{
    if ((*cursor_col) > 0) {
        (*cursor_col)--;

        // Slide all input one character to the left.
        for (size_t i = (*cursor_col); i < (*count); ++i) {
            buffer[i] = buffer[i+1];
            console->draw_char(cursor_row, i + cursor_col_offset,
                               console->get_char(cursor_row, i + cursor_col_offset + 1));
        }
        console->draw_char(cursor_row, (*count) + cursor_col_offset, console->make_char(' '));
        buffer[*count] = '\0';

        if ((*count) > 0) {
            (*count)--;
        }

        console->set_cursor_position(cursor_row, (*cursor_col) + cursor_col_offset);
    }
}

static void type_character(console_interface_t *console,
                           char *buffer,
                           size_t maxcount,
                           size_t *count,
                           size_t *cursor_col,
                           size_t cursor_row,
                           size_t cursor_col_offset,
                           char ch)
{
    // TODO: It's not a given that typing one character always moves forward one
    //       column. For example, if we type a TAB character then we should step
    //       forward until the next tab-stop.

    if (((*count) < maxcount) && console->is_acceptable(ch)) {
        // Slide all input one character to the right.
        if ((*count) > 0 && (*cursor_col) < (*count)-1) {
            for (size_t i = (*count); i > (*cursor_col); --i) {
                buffer[i] = buffer[i-1];
                console->draw_char(cursor_row, i + cursor_col_offset,
                                   console->get_char(cursor_row, i + cursor_col_offset - 1));
            }
        }
        console->draw_char(cursor_row, (*cursor_col) + cursor_col_offset, console->make_char(ch));
        buffer[*cursor_col] = ch;

        (*count)++;
        (*cursor_col)++;
        console->set_cursor_position(cursor_row, (*cursor_col) + cursor_col_offset);
    }
}

static void replace_entire_line(char *replacement,
                                console_interface_t *console,
                                char *buffer,
                                size_t maxcount,
                                size_t *count,
                                size_t *cursor_col,
                                size_t cursor_row,
                                size_t cursor_col_offset)
{
    size_t n = strlen(replacement);

    // Backspace the entire current line.
    for (size_t i = 0; i < n; ++i) {
        backspace(console,
                  buffer,
                  count,
                  cursor_col,
                  cursor_row,
                  cursor_col_offset);
    }

    // Re-type the history line.
    for (size_t i = 0; i < n; ++i) {
        char ch = replacement[i];
        type_character(console,
                       buffer,
                       maxcount,
                       count,
                       cursor_col,
                       cursor_row,
                       cursor_col_offset,
                       ch);
    }
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

    this->history_cursor = -1;
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

            case KEYCODE_NUMPAD_2:
                // fall through

            case KEYCODE_DOWN_ARROW:
                if (this->history_cursor > 0) {
                    this->history_cursor--;
                    char *history = ll_at_str(this->history, this->history_cursor);
                    replace_entire_line(history,
                                        this->console,
                                        buffer,
                                        maxcount,
                                        &count,
                                        &cursor_col,
                                        cursor_row,
                                        cursor_col_offset);
                } 
                break;

            case KEYCODE_NUMPAD_8:
                // fall through

            case KEYCODE_UP_ARROW:
                {
                    int history_count = (int)ll_count_str(this->history);
                    if (this->history_cursor+1 < history_count) {
                        this->history_cursor++;

                        char *history = ll_at_str(this->history, this->history_cursor);
                        replace_entire_line(history,
                                            this->console,
                                            buffer,
                                            maxcount,
                                            &count,
                                            &cursor_col,
                                            cursor_row,
                                            cursor_col_offset);
                    }
                } 
                break;

            default:
                this->history_cursor = -1;

                switch (ch) {
                    case '\b':
                        backspace(this->console,
                                  buffer,
                                  &count,
                                  &cursor_col,
                                  cursor_row,
                                  cursor_col_offset);
                        break;

                    case '\n':
                        buffer[count] = '\0';
                        cursor_col = count;
                        this->console->set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
                        this->console->putchar('\n');
                        have_a_newline = true;
                        break;

                    default:
                        type_character(this->console,
                                       buffer,
                                       maxcount,
                                       &count,
                                       &cursor_col,
                                       cursor_row,
                                       cursor_col_offset,
                                       ch);
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

// Add a line to the editor history.
static void line_editor_add_history(line_editor_impl_t *this,
                                    const char *line_of_history)
{
    assert(this);
    assert(line_of_history);

    size_t n = strlen(line_of_history) + 1;
    char *the_copy = malloc(n);
    memcpy(the_copy, line_of_history, n);
    the_copy[n] = 0;

    ll_push_front_str(this->history, the_copy);

    // for (int i = 0, n = (int)ll_count_str(this->history); i < n; ++i) {
    //     console_printf(this->console, "history %d -- %s\n", i, ll_at_str(this->history, i));
    // }
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
    this->add_history = line_editor_add_history;

    this->console = console;
    this->keyboard = keyboard;
    this->prompt_size = 0;
    this->prompt = NULL;
    this->history = ll_init_str();
    this->history_cursor = -1;

    static const char default_prompt[] = ">";
    line_editor_set_prompt(this, sizeof(default_prompt), default_prompt);

    return (line_editor_t *)this;
}