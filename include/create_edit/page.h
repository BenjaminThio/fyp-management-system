#pragma once

#include <sstream>
#include <array>
#include "json.h"
using namespace std;

namespace create_edit_fyp {
    enum class Subpage: int {
        CREATE,
        EDIT
    };
    enum class CreateField: int {
        TITLE,
        DESCRIPTION,
        ASSIGN_MODERATOR,
        CREATE
    };
    enum class EditField: int {
        TITLE,
        DESCRIPTION,
        ASSIGN_MODERATOR,
        SAVE
    };

    extern bool subpage;
    extern json mod;

    string get_fyp_id();
    bool get_subpage();
    void create();
    void edit(const string& fyp_id);
    void push_frame(ostringstream& renderer, array<int, 2>& manual_input_cursor_pos);
    void keyboard_input_callback(void);
    void caret_handler(void);
}