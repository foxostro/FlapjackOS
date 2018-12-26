#ifndef FLAPJACKOS_COMMON_INCLUDE_LINE_EDITOR_HPP
#define FLAPJACKOS_COMMON_INCLUDE_LINE_EDITOR_HPP

#include <cstddef>

#include <common/linked_list.hpp>
#include <common/text_terminal.hpp>
#include <common/keyboard_device.hpp>

// Maxium size of a prompt in LineEditor.
static constexpr int MAXPROMPT = 3;

// This can be used to read lines of user input from the terminal and provides
// useful UI affordances such as nice editing functionality, a prompt, history,
class LineEditor {
public:
    using Line = RingBuffer<char, CONSOLE_WIDTH-MAXPROMPT>;

    ~LineEditor();

    // Constructor.
    // and such.
    // display -- The text terminal where we display characters.
    // kb -- The keyboard device which gives us user input.
    LineEditor(TextTerminal &terminal, KeyboardDevice &keyboard);

    // Prompt for one line of user input on the console.
    // Returns a string containing the line of user input.
    Line getline();

    // Change the prompt displayed at the beginning of the line.
    // The prompt may not be longer than MAXPROMPT characters.
    void set_prompt(size_t size, const char *prompt);

    // Add a line to the editor's history.
    void add_history(Line entry);

private:
    TextTerminal &terminal_;
    KeyboardDevice &keyboard_;
    RingBuffer<char, MAXPROMPT> prompt_;
    LinkedList<Line> history_;
};

#endif // FLAPJACKOS_COMMON_INCLUDE_LINE_EDITOR_HPP
