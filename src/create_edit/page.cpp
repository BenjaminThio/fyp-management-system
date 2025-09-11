#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <conio.h>
#include "keyboard.h"
#include "globals.h"
#include "input.h"
#include "renderer.h"
#include "audio.h"
#include <array>
#include "create_edit/page.h"
#include "text_formatter.h"
#include "magic_enum.hpp"
#include "json.h"
#include "database.h"
#include "table.h"
#include "renderer.h"
#include "terminal.h"
#include "uuid_v4.h"
using namespace std;
using namespace ansi;
using namespace magic_enum;
using namespace globals;

namespace create_edit_fyp {
    bool subpage = static_cast<bool>(Subpage::CREATE);
    static int field_index = 0;
    static array<input_header, 3> inputs = {
        input_header("", 0, 0, 10, 50, 50, false),
        input_header("", 0, 0, 10, 50, 50, false)
    };
    static bool is_password_visible = false;

    void caret_handler() {
        switch (field_index) {
            case 0:
            case 1:
                terminal::show_cursor();
                break;
            case 2:
                terminal::hide_cursor();
                break;
        }
    }

    void push_frame(ostringstream& renderer, array<int, 2>& manual_cursor_input_pos) {
        renderer << generate_table({
            { " __    ___   ____   __   _____  ____ \n/ /`  | |_) | |_   / /\\   | |  | |_  \n\\_\\_, |_| \\ |_|__ /_/--\\  |_|  |_|__"  },
            { 
                "Title:\n" + render_input_field(inputs[0].field, inputs[0].local_caret_pos, inputs[0].input_field_view_offset, inputs[0].length, inputs[0].error ? BG_RED : BG_WHITE)
            },
            {
                "Description:\n" + render_input_field(inputs[1].field, inputs[1].local_caret_pos, inputs[1].input_field_view_offset, inputs[1].length, inputs[2].error ? BG_RED : BG_WHITE)
            },
            { (field_index == 2 ? format("Create", UNDERLINE) : "Create") },
        });

        if (field_index < 2)
            manual_cursor_input_pos = { inputs[field_index].local_caret_pos + 1, (field_index * 3) + 12 };
    }

    void keyboard_input_callback() {
        int key = -1;
        int special_key = -1;

        if (field_index < 3) {
            array<int, 2> keyboard_input = input_field(inputs[field_index].field, inputs[field_index].local_caret_pos, inputs[field_index].input_field_view_offset, true, inputs[field_index].max_length, inputs[field_index].length);

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
                        field_index = (subpage ? enum_count<EditField>() : enum_count<CreateField>()) - 1;
                } else {
                    if (field_index + 1 < (subpage ? enum_count<EditField>() : enum_count<CreateField>()))
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
                    case static_cast<int>(Subpage::CREATE):
                        switch (field_index) {
                            case static_cast<int>(CreateField::CREATE): {
                                for (size_t i = 0; i < inputs.size(); i++) 
                                    inputs[i].error = inputs[i].field.length() < inputs[i].min_length || inputs[i].field.length() > inputs[i].max_length;

                                for (const input_header& input : inputs) {
                                    if (input.error) {
                                        render_page();
                                        return;
                                    }
                                }

                                json j = load("../data/fyp.json");

                                if (j.contains(json(inputs[0].field))) {
                                    return;
                                }

                                if (j.is_null()) j = json::dictionary{};

                                if (j.is_dictionary()) {
                                    j[UUIDv4::generate()] = json::dictionary{
                                        {
                                            "info", json::dictionary{
                                                { "category", "TESTING123" },
                                                { "description", inputs[1].field },
                                                { "name", inputs[0].field }
                                            }
                                        },
                                        { "isPublic", true },
                                        { "ownerUUID", session_id },
                                        { "wishlistUUIDs", json::list() }
                                    };
                                } else throw runtime_error("The fyp data is not a dictionary.");

                                save("../data/fyp.json", j);
                                fyps = load("../data/fyp.json");
                                console::refresh_fyps_data();

                                redirect(static_cast<int>(Page::CONSOLE));
                                break;
                            }
                        }
                        break;
                    case static_cast<int>(Subpage::EDIT):
                        switch (field_index) {
                            case static_cast<int>(EditField::EDIT):
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
                            field_index = (subpage ? enum_count<EditField>() : enum_count<CreateField>()) - 1;
                        break;
                    case static_cast<int>(Key::DOWN):
                        if (field_index + 1 < (subpage ? enum_count<EditField>() : enum_count<CreateField>()))
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