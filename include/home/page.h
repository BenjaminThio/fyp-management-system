#pragma once

#include <sstream>
using namespace std;

namespace home {
    enum class Option: int {
        FYP_LIST,
        CONSOLE,
        ACCOUNT,
        EXIT
    };

    void push_frame(ostringstream& renderer);
    void keyboard_input_callback(void);
}