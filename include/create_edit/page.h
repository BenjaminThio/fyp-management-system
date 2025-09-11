#pragma once

#include <sstream>
#include <array>
using namespace std;

namespace create_edit_fyp {
    enum class Subpage: int {
        CREATE,
        EDIT
    };
    enum class CreateField: int {
        TITLE,
        DESCRIPTION,
        CREATE
    };
    enum class EditField: int {
        TITLE,
        DESCRIPTION,
        EDIT
    };

    extern bool subpage;

    void push_frame(ostringstream& renderer, array<int, 2>& manual_input_cursor_pos);
    void keyboard_input_callback(void);
    void caret_handler(void);
}