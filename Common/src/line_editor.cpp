#include <common/line_editor.hpp>
#include <common/minmax.hpp>

#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cctype>

line_editor::~line_editor() = default;

line_editor::line_editor(text_terminal &t,
                         keyboard_device &keyboard)
 : term(t),
   kb(keyboard)
{
    set_prompt(vector<char>(strlen(">")+1, ">"));
}

// Prompt for one line of user input on the console.
vector<char> line_editor::getline()
{
    bool have_a_newline = false;

    term.puts(prompt);
    term.putchar(' ');

    vector<char> user_input;

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
                // TODO: left arrow
                break;

            case KEYCODE_NUMPAD_6:
                // fall through

            case KEYCODE_RIGHT_ARROW:
                // TODO: right arrow
                break;

            case KEYCODE_NUMPAD_2:
                // fall through

            case KEYCODE_DOWN_ARROW:
                // TODO: down arrow
                break;

            case KEYCODE_NUMPAD_8:
                // fall through

            case KEYCODE_UP_ARROW:
                // TODO: up arrow
                break;

            default:
                switch (ch) {
                    case '\b':
                        if (user_input.size() > 0) {
                            user_input.pop_back();
                            term.putchar('\b');
                        }
                        break;

                    case '\n':
                        term.putchar('\n');
                        have_a_newline = true;
                        break;

                    case '\t':
                        // TODO: better handling of the tab character
                        user_input.push_back(' ');
                        term.putchar(' ');
                        break;

                    default:
                        if (isprint(ch)) {
                            user_input.push_back(ch);
                            term.putchar(ch);
                        }
                        break;
                } // switch (ch)
                break;
        } // switch (key)
    } // while

    user_input.push_back('\0');
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
