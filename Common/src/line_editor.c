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

static void line_editor_destroy(line_editor_impl_t *self)
{
    while (ll_count_str(self->history) > 0) {
        char *history = ll_remove_str(self->history, 0);
        free(history);
    }
    ll_destroy_str(self->history);

    free(self->prompt);

    free(self);
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
static char * line_editor_getline(line_editor_impl_t *self)
{
    assert(self);
    assert(self->console);
    assert(self->keyboard);
    assert(self->prompt);

    static const size_t buffer_size = 512;
    char *buffer = malloc(buffer_size);

    bool have_a_newline = false;
    size_t count = 0, cursor_col = 0;
    size_t prompt_len = strnlen(self->prompt, self->prompt_size);
    size_t maxcount = MIN(CONSOLE_WIDTH - prompt_len - 1, buffer_size);

    self->history_cursor = -1;
    self->console->puts(self->prompt);
    self->console->putchar(' ');
    size_t cursor_row = self->console->get_cursor_row();
    size_t cursor_col_offset = self->console->get_cursor_col();

    while (!have_a_newline) {
        keyboard_event_t event;
        self->keyboard->get_event(self->keyboard, &event);

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
                    self->console->set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
                }
                break;

            case KEYCODE_NUMPAD_6:
                // fall through

            case KEYCODE_RIGHT_ARROW:
                if (cursor_col < count) {
                    cursor_col++;
                    self->console->set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
                }
                break;

            case KEYCODE_NUMPAD_2:
                // fall through

            case KEYCODE_DOWN_ARROW:
                if (self->history_cursor > 0) {
                    self->history_cursor--;
                    char *history = ll_at_str(self->history, self->history_cursor);
                    replace_entire_line(history,
                                        self->console,
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
                    int history_count = (int)ll_count_str(self->history);
                    if (self->history_cursor+1 < history_count) {
                        self->history_cursor++;

                        char *history = ll_at_str(self->history, self->history_cursor);
                        replace_entire_line(history,
                                            self->console,
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
                self->history_cursor = -1;

                switch (ch) {
                    case '\b':
                        backspace(self->console,
                                  buffer,
                                  &count,
                                  &cursor_col,
                                  cursor_row,
                                  cursor_col_offset);
                        break;

                    case '\n':
                        buffer[count] = '\0';
                        cursor_col = count;
                        self->console->set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
                        self->console->putchar('\n');
                        have_a_newline = true;
                        break;

                    default:
                        type_character(self->console,
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
static void line_editor_set_prompt(line_editor_impl_t *self,
                                   size_t prompt_size, const char *prompt)
{
    assert(self);
    assert(prompt_size > 0);
    assert(prompt);

    free(self->prompt);

    // Let's always allocate one more byte then requested just to make sure we
    // always have a nul-terminator at the end of the string.
    self->prompt = malloc(prompt_size+1);
    memset(self->prompt, 0, prompt_size+1);
    memcpy(self->prompt, prompt, prompt_size);
    self->prompt_size = prompt_size;
}

// Add a line to the editor history.
static void line_editor_add_history(line_editor_impl_t *self,
                                    const char *line_of_history)
{
    assert(self);
    assert(line_of_history);

    size_t n = strlen(line_of_history) + 1;
    char *the_copy = malloc(n);
    memcpy(the_copy, line_of_history, n);
    the_copy[n] = 0;

    ll_push_front_str(self->history, the_copy);

    // for (int i = 0, n = (int)ll_count_str(self->history); i < n; ++i) {
    //     console_printf(self->console, "history %d -- %s\n", i, ll_at_str(self->history, i));
    // }
}

// Returns a new initialized line_editor object.
line_editor_t* line_editor_init(console_interface_t *console,
                                keyboard_interface_t *keyboard)
{
    assert(console);
    assert(keyboard);

    line_editor_impl_t *self = malloc(sizeof(line_editor_impl_t));

    self->destroy = line_editor_destroy;
    self->getline = line_editor_getline;
    self->set_prompt = line_editor_set_prompt;
    self->add_history = line_editor_add_history;

    self->console = console;
    self->keyboard = keyboard;
    self->prompt_size = 0;
    self->prompt = NULL;
    self->history = ll_init_str();
    self->history_cursor = -1;

    static const char default_prompt[] = ">";
    line_editor_set_prompt(self, sizeof(default_prompt), default_prompt);

    return (line_editor_t *)self;
}