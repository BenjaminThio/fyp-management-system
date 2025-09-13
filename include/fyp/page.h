#pragma once

#include <sstream>
using namespace std;

namespace fyp {
    enum class Unit: int {
        LIST,
        VIEW
    };
    enum class ViewOption: int {
        SHORTLIST  
    };
    
    void view(const string& fyp_id);
    void push_frame(ostringstream& renderer, array<int, 2>& manual_cursor_input_pos);
    void create_default_fyp_data(void);
    void keyboard_input_callback(void);
    void caret_handler();
}