#include <common/line_editor.hpp>
#include <common/minmax.hpp>

#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cctype>

LineEditor::~LineEditor() = default;

LineEditor::LineEditor(TextTerminal &t, KeyboardDevice &keyboard)
 : terminal_(t),
   keyboard_(keyboard)
{
    set_prompt(strlen(">"), ">");
}

// Prompt for one line of user input on the console.
LineEditor::Line LineEditor::getline()
{
    bool have_a_newline = false;

    terminal_.putv(prompt_);
    terminal_.putchar(' ');

    Line user_input, stashed_user_input;
    Line::size_type linear_cursor = 0;
    int history_cursor = -1;

    while (!have_a_newline) {
        KeyboardEvent event = keyboard_.get_event();

        if (event.state != PRESSED) {
            continue;
        }

        char ch = event.ch;
        keycode_t key = event.key;

        switch (key) {
            case KEYCODE_NUMPAD_4:
                // fall through

            case KEYCODE_LEFT_ARROW:
                if (linear_cursor > 0) {
                    linear_cursor--;
                    terminal_.move_cursor_left();
                }
                break;

            case KEYCODE_NUMPAD_6:
                // fall through

            case KEYCODE_RIGHT_ARROW:
                if (linear_cursor < user_input.size()) {
                    linear_cursor++;
                    terminal_.move_cursor_right();
                }
                break;

            case KEYCODE_NUMPAD_2:
                // fall through

            case KEYCODE_DOWN_ARROW:
                if (history_cursor != -1) {
                    terminal_.move_cursor_to_end();
                    for (int i = 0, n = user_input.size(); i < n; ++i) {
                        terminal_.putchar('\b');
                    }
                    history_cursor = MAX(history_cursor-1, -1);
                    if (history_cursor == -1) {
                        user_input = stashed_user_input;
                    } else {
                        user_input = history_[history_cursor];
                    }
                    for (int i = 0, n = user_input.size(); i < n; ++i) {
                        terminal_.putchar(user_input[i]);
                    }
                    linear_cursor = user_input.size();
                }
                break;

            case KEYCODE_NUMPAD_8:
                // fall through

            case KEYCODE_UP_ARROW:
                if (history_cursor < history_.size()-1) {
                    terminal_.move_cursor_to_end();
                    for (int i = 0, n = user_input.size(); i < n; ++i) {
                        terminal_.putchar('\b');
                    }
                    if (history_cursor == -1) {
                        stashed_user_input = user_input;
                    }
                    history_cursor = MIN(history_cursor+1, history_.size()-1);
                    if (history_cursor == -1) {
                        user_input = stashed_user_input;
                    } else {
                        user_input = history_[history_cursor];
                    }
                    for (int i = 0, n = user_input.size(); i < n; ++i) {
                        terminal_.putchar(user_input[i]);
                    }
                    linear_cursor = user_input.size();
                }
                break;

            default:
                switch (ch) {
                    case '\b':
                        if (user_input.size() > 0) {
                            user_input.remove(--linear_cursor);
                            terminal_.putchar('\b');
                        }
                        break;

                    case '\n':
                        terminal_.putchar('\n');
                        have_a_newline = true;
                        break;

                    case '\t':
                    default:
                        if ((isprint(ch) || ch == '\t') &&
                            user_input.size() < MAXLINE) {

                            user_input.insert(linear_cursor++, ch);
                            terminal_.putchar(ch);
                        }
                        break;
                } // switch (ch)
                break;
        } // switch (key)
    } // while

    return user_input;
}

// Change the prompt displayed at the beginning of the line.
void LineEditor::set_prompt(size_t size, const char *str)
{
    prompt_.clear();
    for (int i = 0, n = MIN(MAXPROMPT, (int)strnlen(str, size)); i < n; ++i) {
        prompt_.push_back(str[i]);
    }
}

// Add a line to the editor history.
void LineEditor::add_history(Line entry)
{
    history_.push_front(std::move(entry));
}
