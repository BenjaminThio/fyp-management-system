#pragma once

#include <sstream>
using namespace std;

namespace fyp {
    enum class Unit: int {
        LIST,
        VIEW
    };

    void start();
    void push_frame(ostringstream& renderer);
    void create_default_fyp_data(void);
    void keyboard_input_callback(void);
}