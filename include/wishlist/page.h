#pragma once

#include <sstream>
using namespace std;

namespace wishlist {
    void push_frame(ostringstream& renderer);
    void keyboard_input_callback();
}