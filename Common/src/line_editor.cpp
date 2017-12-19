#include <line_editor.hpp>

#include <cstdlib>
#include <cstring>
#include <cassert>

// #include <console_printf.h>

#define MIN(a, b) \
   ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
      _a < _b ? _a : _b; })

#define MAX(a, b) \
   ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
      _a > _b ? _a : _b; })

line_editor::~line_editor()
{
    while (ll_count_str(history) > 0) {
        char *str = ll_remove_str(history, 0);
        free(str);
    }
    ll_destroy_str(history);

    free(prompt);
}

line_editor::line_editor(console_interface_t *_console,
                         keyboard_device &_keyboard)
 : con(_console),
   kb(_keyboard),
   prompt_size(0),
   prompt(NULL),
   history(ll_init_str()),
   history_cursor(-1)
{
    assert(_console);

    static const char default_prompt[] = ">";
    set_prompt(sizeof(default_prompt), default_prompt);
}

void line_editor::backspace(char *buffer,
                            size_t &count,
                            size_t &cursor_col,
                            size_t cursor_row,
                            size_t cursor_col_offset)
{
    if (cursor_col > 0) {
        cursor_col--;

        // Slide all input one character to the left.
        for (size_t i = cursor_col; i < count; ++i) {
            buffer[i] = buffer[i+1];
            con->draw_char(cursor_row, i + cursor_col_offset,
                           con->get_char(cursor_row, i + cursor_col_offset + 1));
        }
        con->draw_char(cursor_row, count + cursor_col_offset, con->make_char(' '));
        buffer[count] = '\0';

        if (count > 0) {
            count--;
        }

        con->set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
    }
}

void line_editor::type_character(char *buffer,
                                 size_t maxcount,
                                 size_t &count,
                                 size_t &cursor_col,
                                 size_t cursor_row,
                                 size_t cursor_col_offset,
                                 char ch)
{
    // TODO: It's not a given that typing one character always moves forward one
    //       column. For example, if we type a TAB character then we should step
    //       forward until the next tab-stop.

    if ((count < maxcount) && con->is_acceptable(ch)) {
        // Slide all input one character to the right.
        if (count > 0 && cursor_col < count-1) {
            for (size_t i = count; i > cursor_col; --i) {
                buffer[i] = buffer[i-1];
                con->draw_char(cursor_row, i + cursor_col_offset,
                               con->get_char(cursor_row, i + cursor_col_offset - 1));
            }
        }
        con->draw_char(cursor_row, cursor_col + cursor_col_offset, con->make_char(ch));
        buffer[cursor_col] = ch;

        count++;
        cursor_col++;
        con->set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
    }
}

void line_editor::replace_entire_line(char *replacement,
                                      char *buffer,
                                      size_t maxcount,
                                      size_t &count,
                                      size_t &cursor_col,
                                      size_t cursor_row,
                                      size_t cursor_col_offset)
{
    size_t n = strlen(replacement);

    // Backspace the entire current line.
    for (size_t i = 0; i < n; ++i) {
        backspace(buffer,
                  count,
                  cursor_col,
                  cursor_row,
                  cursor_col_offset);
    }

    // Re-type the history line.
    for (size_t i = 0; i < n; ++i) {
        char ch = replacement[i];
        type_character(buffer,
                       maxcount,
                       count,
                       cursor_col,
                       cursor_row,
                       cursor_col_offset,
                       ch);
    }
}

// Prompt for one line of user input on the console.
char * line_editor::getline()
{
    assert(prompt);

    static const size_t buffer_size = 512;
    char *buffer = (char *)malloc(buffer_size);

    bool have_a_newline = false;
    size_t count = 0, cursor_col = 0;
    size_t prompt_len = strnlen(prompt, prompt_size);
    size_t maxcount = MIN(CONSOLE_WIDTH - prompt_len - 1, buffer_size);

    history_cursor = -1;
    con->puts(prompt);
    con->putchar(' ');
    size_t cursor_row = con->get_cursor_row();
    size_t cursor_col_offset = con->get_cursor_col();

    while (!have_a_newline) {
        keyboard_event event = kb.get_event();

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
                    con->set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
                }
                break;

            case KEYCODE_NUMPAD_6:
                // fall through

            case KEYCODE_RIGHT_ARROW:
                if (cursor_col < count) {
                    cursor_col++;
                    con->set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
                }
                break;

            case KEYCODE_NUMPAD_2:
                // fall through

            case KEYCODE_DOWN_ARROW:
                if (history_cursor > 0) {
                    history_cursor--;
                    char *str = ll_at_str(history, history_cursor);
                    replace_entire_line(str,
                                        buffer,
                                        maxcount,
                                        count,
                                        cursor_col,
                                        cursor_row,
                                        cursor_col_offset);
                } 
                break;

            case KEYCODE_NUMPAD_8:
                // fall through

            case KEYCODE_UP_ARROW:
                {
                    int history_count = (int)ll_count_str(history);
                    if (history_cursor+1 < history_count) {
                        history_cursor++;

                        char *str = ll_at_str(history, history_cursor);
                        replace_entire_line(str,
                                            buffer,
                                            maxcount,
                                            count,
                                            cursor_col,
                                            cursor_row,
                                            cursor_col_offset);
                    }
                } 
                break;

            default:
                history_cursor = -1;

                switch (ch) {
                    case '\b':
                        backspace(buffer,
                                  count,
                                  cursor_col,
                                  cursor_row,
                                  cursor_col_offset);
                        break;

                    case '\n':
                        buffer[count] = '\0';
                        cursor_col = count;
                        con->set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
                        con->putchar('\n');
                        have_a_newline = true;
                        break;

                    default:
                        type_character(buffer,
                                       maxcount,
                                       count,
                                       cursor_col,
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
void line_editor::set_prompt(size_t new_prompt_size, const char *new_prompt)
{
    assert(new_prompt_size > 0);
    assert(new_prompt);

    free(prompt);

    // Let's always allocate one more byte then requested just to make sure we
    // always have a nul-terminator at the end of the string.
    prompt_size = new_prompt_size;
    prompt = (char *)malloc(prompt_size+1);
    memset(prompt, 0, prompt_size+1);
    memcpy(prompt, new_prompt, prompt_size);
}

// Add a line to the editor history.
void line_editor::add_history(const char *line_of_history)
{
    assert(line_of_history);

    size_t n = strlen(line_of_history) + 1;
    char *the_copy = (char *)malloc(n);
    memcpy(the_copy, line_of_history, n);
    the_copy[n] = 0;

    ll_push_front_str(history, the_copy);

    // for (int i = 0, n = (int)ll_count_str(history); i < n; ++i) {
    //     console_printf(con, "history %d -- %s\n", i, ll_at_str(history, i));
    // }
}
