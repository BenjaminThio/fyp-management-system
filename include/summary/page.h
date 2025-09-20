#pragma once

#include <sstream>
using namespace std;

namespace summary {
    void summary(const string& fyp_id);
    void push_frame(ostringstream& renderer);
    void keyboard_input_callback();
}