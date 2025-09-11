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
        GRADING,
        SUBMIT,
        TEST,
        EXIT
    };
    enum class UnauthorizedOption: int {
        SIGN_UP,
        LOGIN,
        EXIT
    };
    enum class StudentOption: int {
        FYP_LIST,
        WISHLIST,
        SUBMIT,
        LOG_OUT,
        EXIT
    };
    enum class AdminOption: int {
        FYP_LIST,
        CONSOLE,
        GRADING,
        LOG_OUT,
        EXIT
    };

    extern size_t id;
    extern string frame_chunk;
    extern bool is_initialized;
    extern map<int, bool> active_tasks;

    void start(void);
    void push_frame(ostringstream& renderer);
    void keyboard_input_callback(void);
}