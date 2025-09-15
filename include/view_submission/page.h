#pragma once

#include <sstream>
#include <array>
using namespace std;

namespace view_submission {
    json get_submitter();
    void refresh_submitters();
    void caret_handler();
    void push_frame(ostringstream& renderer, array<int, 2>& manual_cursor_input_pos);
    void keyboard_input_callback();
}