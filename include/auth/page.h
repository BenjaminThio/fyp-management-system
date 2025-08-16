#pragma once

#include <sstream>
#include <array>
using namespace std;

namespace sign_up {
    /*
    enum class Field: int {
        USERNAME,
        PASSWORD,
        EMAIL
    };
    */

    void push_frame(ostringstream& renderer, array<int, 2>& manual_input_cursor_pos);
    void keyboard_input_callback(void);
}