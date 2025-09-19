#pragma once

#include <sstream>
using namespace std;

namespace grading {
    void start();
    void field_coord_correction();
    void push_frame(ostringstream& renderer, array<int, 2>& manual_cursor_input_pos);
    void keyboard_input_callback();
}