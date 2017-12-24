#include <common/line_editor.hpp>
#include <common/minmax.hpp>

#include <cstdlib>
#include <cstring>
#include <cassert>

line_editor::~line_editor() = default;

line_editor::line_editor(console_device &console,
                         keyboard_device &keyboard)
 : con(console),
   kb(keyboard),
   history_cursor(-1)
{
    set_prompt(">");
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
            con.draw_char(cursor_row, i + cursor_col_offset,
                          con.get_char(cursor_row, i + cursor_col_offset + 1));
        }
        con.draw_char(cursor_row, count + cursor_col_offset, con.make_char(' '));
        buffer[count] = '\0';

        if (count > 0) {
            count--;
        }

        con.set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
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

    if ((count < maxcount) && con.is_acceptable(ch)) {
        // Slide all input one character to the right.
        if (count > 0 && cursor_col < count-1) {
            for (size_t i = count; i > cursor_col; --i) {
                buffer[i] = buffer[i-1];
                con.draw_char(cursor_row, i + cursor_col_offset,
                              con.get_char(cursor_row, i + cursor_col_offset - 1));
            }
        }
        con.draw_char(cursor_row, cursor_col + cursor_col_offset, con.make_char(ch));
        buffer[cursor_col] = ch;

        count++;
        cursor_col++;
        con.set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
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
vector<char> line_editor::getline()
{
    bool have_a_newline = false;
    size_t count = 0, cursor_col = 0;
    size_t prompt_len = strnlen(prompt.data(), prompt.size());

    static const size_t buffer_size = 512;
    size_t maxcount = MIN(CONSOLE_WIDTH - prompt_len - 1, buffer_size);
    vector<char> user_input(maxcount);

    history_cursor = -1;
    con.puts(prompt.data());
    con.putchar(' ');
    size_t cursor_row = con.get_cursor_row();
    size_t cursor_col_offset = con.get_cursor_col();

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
                    con.set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
                }
                break;

            case KEYCODE_NUMPAD_6:
                // fall through

            case KEYCODE_RIGHT_ARROW:
                if (cursor_col < count) {
                    cursor_col++;
                    con.set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
                }
                break;

            case KEYCODE_NUMPAD_2:
                // fall through

            case KEYCODE_DOWN_ARROW:
                if (history_cursor > 0) {
                    history_cursor--;
                    vector<char> history_line = history.at(history_cursor);
                    replace_entire_line(history_line.data(),
                                        user_input.data(),
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
                    int history_count = history.count();
                    if (history_cursor+1 < history_count) {
                        history_cursor++;

                        vector<char> history_line = history.at(history_cursor);
                        replace_entire_line(history_line.data(),
                                            user_input.data(),
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
                        backspace(user_input.data(),
                                  count,
                                  cursor_col,
                                  cursor_row,
                                  cursor_col_offset);
                        break;

                    case '\n':
                        user_input[count] = '\0';
                        cursor_col = count;
                        con.set_cursor_position(cursor_row, cursor_col + cursor_col_offset);
                        con.putchar('\n');
                        have_a_newline = true;
                        break;

                    default:
                        type_character(user_input.data(),
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

    return user_input;
}

// Change the prompt displayed at the beginning of the line.
void line_editor::set_prompt(vector<char> new_prompt)
{
    prompt = new_prompt;
}

// Add a line to the editor history.
void line_editor::add_history(vector<char> history_entry)
{
    history.push_front(history_entry);
}
