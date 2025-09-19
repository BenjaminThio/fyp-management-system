#pragma once

#include <sstream>
#include <array>
using namespace std;

namespace auth {
    enum class Subpage: int {
        SIGN_UP,
        LOGIN
    };
    enum class LoginField: int {
        EMAIL,
        PASSWORD,
        LOGIN,
        SIGN_UP
    };
    enum class SignUpField: int {
        USERNAME,
        PASSWORD,
        EMAIL,
        SIGN_UP,
        LOGIN
    };

    extern bool subpage;

    void push_frame(ostringstream& renderer, array<int, 2>& manual_input_cursor_pos);
    void keyboard_input_callback(void);
    void caret_handler(void);
    void sign_up_init();
    void login_init();
}