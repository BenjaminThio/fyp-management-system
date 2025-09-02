#pragma once

#include <sstream>
using namespace std;

namespace grading {
    void push_frame(ostringstream& renderer);
    void keyboard_input_callback();
}