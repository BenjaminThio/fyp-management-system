#pragma once

#include <sstream>
using namespace std;

namespace fyp_view {
    void push_frame(ostringstream& renderer);
    void keyboard_input_callback();
}