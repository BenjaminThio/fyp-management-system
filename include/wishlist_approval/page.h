#pragma once

#include <string>
#include <sstream>
using namespace std;

namespace wishlist_approval {
    void refresh_wishlist_data();
    void wishlist_approval(const string& fyp_id);
    void push_frame(ostringstream& renderer);
    void keyboard_input_callback();
}