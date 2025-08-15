#pragma once

#include <sstream>
using namespace std;

namespace console {
    void start(void);
    void push_frame(ostringstream& renderer);
    void keyboard_input_callback(void);
}