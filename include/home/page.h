#pragma once

#include <sstream>
#include <map>
using namespace std;

namespace home {
    enum class Option: int {
        FYP_LIST,
        CONSOLE,
        ACCOUNT,
        LOGIN,
        WISHLIST,
        CREATE_EDIT,
        EXIT
    };

    extern size_t id;
    extern string frame_chunk;
    extern bool is_initialized;
    extern map<int, bool> active_tasks;

    void push_frame(ostringstream& renderer);
    void keyboard_input_callback(void);
}