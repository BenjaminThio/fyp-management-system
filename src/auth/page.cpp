#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <conio.h>
#include <regex>
#include "keyboard.h"
#include "globals.h"
#include "input.h"
#include "renderer.h"
#include "audio.h"
#include <array>
#include "auth/page.h"
#include "text_formatter.h"
#include "magic_enum.hpp"
#include "json.h"
#include "database.h"
#include "table.h"
#include "renderer.h"
#include "terminal.h"
using namespace std;
using namespace ansi;
using namespace magic_enum;

namespace auth {
    bool subpage = static_cast<bool>(Subpage::SIGN_UP);
    static int field_index = 0;
    static array<input_header, 3> inputs = {
        input_header("", 0, 0, 3, 32, 37, false),
        input_header("", 0, 0, 8, 32, 37, false),
        input_header("", 0, 0, 6, 32, 37, false)
    };
    /*
    static array<string, 3> fields;
    static array<int, 3> cursor_input_pos = { 0, 0, 0 };
    static array<size_t, 3> min_lengths = { 3, 8, 6 };
    static array<size_t, 3> max_lengths = { 32, 32, 32 };
    static array<bool, 3> errors = { false, false, false };
    */
    static bool is_password_visible = false;

    void caret_handler() {
        switch (field_index) {
            case 0: // 0..2
            case 1:
            case 2:
                terminal::show_cursor();
                break;
            case 3: // 3..4
            case 4:
                terminal::hide_cursor();
                break;
        }
    }

    void push_frame(ostringstream& renderer, array<int, 2>& manual_cursor_input_pos) {
        renderer << (
        subpage ? generate_table({
            { "" },
            {
                "Email:\n" + render_input_field(inputs[0].field, inputs[0].local_caret_pos, inputs[0].input_field_view_offset, inputs[0].length, inputs[0].error ? BG_RED : BG_WHITE), generate_table({
                    { "No.", "Guidelines" },
                    { "1.", "The email must contain at least " + to_string(inputs[0].min_length) + " characters and no more than " + to_string(inputs[0].max_length) + " characters." },
                    { "2.", "The email must not include the characters..." }
                })
            },
            {
                "Password:\n" + render_input_field(is_password_visible ? inputs[1].field : string(inputs[1].field.length(), '*'), inputs[1].local_caret_pos, inputs[1].input_field_view_offset, inputs[1].length, inputs[1].error ? BG_RED : BG_WHITE), generate_table({
                    { "No.", "Guidelines" },
                    { "1.", "The password must contain at least " + to_string(inputs[1].min_length) + " characters and no more than " + to_string(inputs[1].max_length) + " characters." },
                    { "2.", "The password must not include the characters..." }
                })
            },
            { (field_index == 2 ? format("Login", UNDERLINE) : "Login") },
            { "Do not own an account? " + (field_index == 3 ? format("Login here", UNDERLINE) : "Login here") + "." }
        })
        : 
        generate_table({
            { " __   _   __    _          _     ___\n( (` | | / /`_ | |\\ |     | | | | |_)\n_)_) |_| \\_\\_/ |_| \\|     \\_\\_/ |_|" },
            { 
                "Username:\n" + render_input_field(inputs[0].field, inputs[0].local_caret_pos, inputs[0].input_field_view_offset, inputs[0].length, inputs[0].error ? BG_RED : BG_WHITE), generate_table({
                    { "No.", "Guidelines" },
                    { "1.", "The username must contain at least " + to_string(inputs[0].min_length) + " characters and no more than " + to_string(inputs[0].max_length) + " characters." },
                    { "2.", "The username must not include the characters..." }
                })
            },
            {
                "Password:\n" + render_input_field(is_password_visible ? inputs[1].field : string(inputs[1].field.length(), '*'), inputs[1].local_caret_pos, inputs[1].input_field_view_offset, inputs[1].length, inputs[1].error ? BG_RED : BG_WHITE), generate_table({
                    { "No.", "Guidelines" },
                    { "1.", "The password must contain at least " + to_string(inputs[1].min_length) + " characters and no more than " + to_string(inputs[1].max_length) + " characters." },
                    { "2.", "The password must not include the characters..." }
                })
            },
            {
                "Email:\n" + render_input_field(inputs[2].field, inputs[2].local_caret_pos, inputs[2].input_field_view_offset, inputs[2].length, inputs[2].error ? BG_RED : BG_WHITE), generate_table({
                    { "No.", "Guidelines" },
                    { "1.", "The email must contain at least " + to_string(inputs[2].min_length) + " characters and no more than " + to_string(inputs[2].max_length) + " characters." },
                    { "2.", "The email must not include the characters..." }
                })
            },
            { (field_index == 3 ? format("Sign Up", UNDERLINE) : "Sign Up") },
            { "Already have an account? " + (field_index == 4 ? format("Login here", UNDERLINE) : "Login here") + "." }
        }));

        /*
        renderer
        << "Username:"
        << endl
        << fields[0]
        << endl
        << "Password:"
        << endl
        << (is_password_visible ? fields[1] : string(fields[1].length(), '*'))
        << endl
        << "Email:"
        << endl
        << fields[2]
        << endl
        << (field_index == 3 ? format("Sign Up", UNDERLINE) : "Sign Up")
        << endl
        << "Already have an account? " << (field_index == 4 ? format("Login here", UNDERLINE) : "Login here") << '.';
        */

        if (field_index < 3)
            manual_cursor_input_pos = { inputs[field_index].local_caret_pos + 1, (field_index * 8) + 12 }; // (field_index * 3) + 12
    }

    void keyboard_input_callback() {
        int key = -1;
        int special_key = -1;

        if (field_index < 3) {
            array<int, 2> keyboard_input = input_field(inputs[field_index].field, inputs[field_index].local_caret_pos, inputs[field_index].input_field_view_offset, field_index != 1, inputs[field_index].max_length, inputs[field_index].length);

            key = keyboard_input[0];
            special_key = keyboard_input[1];
        }
        else if (_kbhit()) {
            key = _getch();

            switch (key) {
                case 0:
                case 224: special_key = _getch();
            }
        }

        switch (key) {
            case static_cast<int>(Key::ESCAPE):
                return_page();
                break;
            case static_cast<int>(Key::TAB):
                if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
                    if (field_index - 1 >= 0)
                        field_index--;
                    else
                        field_index = (subpage ? enum_count<LoginField>() : enum_count<SignUpField>()) - 1;
                } else {
                    if (field_index + 1 < (subpage ? enum_count<LoginField>() : enum_count<SignUpField>()))
                        field_index++;
                    else
                        field_index = 0;
                }

                caret_handler();
                render_page();
                play_sound("squeak");
                break;
            case static_cast<int>(Key::ENTER):
                switch (subpage) {
                    case static_cast<int>(Subpage::SIGN_UP):
                        switch (field_index) {
                            case static_cast<int>(SignUpField::SIGN_UP): {
                                const regex username_regex(R"(^[a-zA-Z]+(?: [a-zA-Z]+)*{8,32}$)");

                                if (!regex_match(inputs[0].field, username_regex))
                                    inputs[0].error = true;
                                else
                                    inputs[0].error = false;

                                const regex password_pattern(R"(^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[~`!@#$%^&*()_\-+={[}\]|\\:;"'<,>.?\/])[^\s]{8,32}$)");
                                
                                if (!regex_match(inputs[1].field, password_pattern))
                                    inputs[1].error = true;
                                else
                                    inputs[1].error = false;

                                const regex email_pattern(R"(^[0-9a-zA-Z]{6,32}@utar(student|admin).com?)");

                                if (!regex_match(inputs[2].field, email_pattern))
                                    inputs[2].error = true;
                                else
                                    inputs[2].error = false;

                                for (const input_header& input : inputs) {
                                    if (input.error) {
                                        render_page();
                                        return;
                                    }
                                }

                                json j = load("../data/user.json");

                                if (j.contains(json(inputs[2].field))) {
                                    return;
                                }

                                if (j.is_null()) j = json::dictionary{};

                                if (j.is_dictionary()) {
                                    j[inputs[2].field] = json::dictionary{
                                        { "fyps", json::list{} },
                                        {
                                            "info", json::dictionary{
                                                { "username", inputs[0].field },
                                                { "password", inputs[1].field },
                                                { "email", inputs[2].field }
                                            }
                                        }
                                    };
                                } else throw runtime_error("The user data is not a dictionary.");

                                save("../data/user.json", j);

                                redirect(static_cast<int>(Page::HOME));
                                break;
                            }
                        }
                        break;
                    case static_cast<int>(Subpage::LOGIN):
                        switch (field_index) {
                            case static_cast<int>(LoginField::LOGIN):
                                break;
                            case static_cast<int>(LoginField::SIGN_UP):
                                break;
                        }
                        break;
                }
                break;
        }

        switch (special_key) {
            case 59: // F1
                is_password_visible = !is_password_visible;

                render_page();
                play_sound("squeak");
                break;
            case static_cast<int>(Key::UP):
            case static_cast<int>(Key::DOWN):
                switch (special_key) {
                    case static_cast<int>(Key::UP):
                        if (field_index - 1 >= 0)
                            field_index--;
                        else
                            field_index = (subpage ? enum_count<LoginField>() : enum_count<SignUpField>()) - 1;
                        break;
                    case static_cast<int>(Key::DOWN):
                        if (field_index + 1 < (subpage ? enum_count<LoginField>() : enum_count<SignUpField>()))
                            field_index++;
                        else
                            field_index = 0;
                        break;
                }

                caret_handler();
                render_page();
                play_sound("squeak");
                break;
        }
    }
}