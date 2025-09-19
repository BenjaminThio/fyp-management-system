#pragma once

#include <sstream>
using namespace std;

namespace view_grading {
    void push_frame(ostringstream& renderer);
    void keyboard_input_callback();
}