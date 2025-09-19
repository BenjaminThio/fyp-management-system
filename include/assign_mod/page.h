#pragma once

#include <sstream>
#include <array>
using namespace std;

namespace assign_mod {
    void reset_data();
    void refresh_admins();
    void caret_handler();
    void push_frame(ostringstream& renderer, array<int, 2>& manual_cursor_input_pos);
    void keyboard_input_callback();
}