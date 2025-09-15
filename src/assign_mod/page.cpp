#include <sstream>
#include <conio.h>
#include <array>
#include "keyboard.h"
#include "globals.h"
#include "renderer.h"
#include "input.h"
#include "terminal.h"
#include "database.h"
#include "search.h"
#include "create_edit/page.h"
using namespace std;

namespace assign_mod {
    static json admins = json::dictionary{};
    static json visible_admins = json::dictionary{};
    static int selected_option = 0;
    static string search_field;
    static int local_caret_pos;
    static int input_field_view_offset;
    static int visible_quantity = 20;
    static int field_length = 30;

    void refresh_admins() {
        json j = load("../data/user.json");

        for (auto& [key, val] : j.as_dictionary()) {
            if (is_admin(key)) {
                admins[key] = val;
            }
        }
    }

    void caret_handler() {
        switch (selected_option) {
            case 0:
                terminal::show_cursor();
                return;
        }
        terminal::hide_cursor();
    }

    void push_frame(ostringstream& renderer, array<int, 2>& manual_cursor_input_pos) {
        renderer << "Search: " << render_input_field(search_field, local_caret_pos, input_field_view_offset, field_length) << endl << endl;

        if (search_field.length() > 0) {
            visible_admins = json::dictionary{};
            
            for (auto& [key, val] : admins.as_dictionary()) {
                if (search::similar(val["info"]["username"].as_string() + " " + val["info"]["email"].as_string(), search_field)) {
                    visible_admins[key] = val;
                }
            }
        }

        size_t counter = 0;

        for (auto& [key, val] : (search_field.length() == 0 ? admins : visible_admins).as_dictionary()) {
            if (counter < visible_quantity) {
                renderer << ((counter++ + 1 == selected_option) ? '>' : ' ') << ' ' << val["info"]["username"].as_string() << endl;
            }
        }

        switch (selected_option) {
            case 0:
                manual_cursor_input_pos = { local_caret_pos + 8, 6 };
                break;
        }
    }

    void keyboard_input_callback() {
        int key = -1;
        int special_key = -1;

        if (selected_option == 0) {
            array<int, 2> keyboard_input = input_field(search_field, local_caret_pos, input_field_view_offset, true, 0, field_length);

            key = keyboard_input[0];
            special_key = keyboard_input[1];
        } else if (selected_option > 0) {
            if (_kbhit()) {
                key = _getch();

                switch (key) {
                    case 0:
                    case 224:
                        special_key = _getch();
                        break;
                }
            }
        }

        switch (special_key) {
            case static_cast<int>(Key::UP): {
                if (selected_option - 1 >= 0) {
                    selected_option--;
                } else {
                    selected_option = (visible_quantity > (search_field.length() == 0 ? admins : visible_admins).size() ? (search_field.length() == 0 ? admins : visible_admins).size() : visible_quantity);
                }
                // play_sound("squeak");
                render_page();
                caret_handler();
                break;
            }
            case static_cast<int>(Key::DOWN): {
                if (selected_option + 1 <= (visible_quantity > (search_field.length() == 0 ? admins : visible_admins).size() ? (search_field.length() == 0 ? admins : visible_admins).size() : visible_quantity)) {
                    selected_option++;
                } else {
                    selected_option = 0;
                }
                // play_sound("squeak");
                render_page();
                caret_handler();
                break;
            }
        }

        switch (key) {
            case static_cast<int>(Key::ENTER): {
                switch (selected_option) {
                    case 0:
                        return;
                }
                fyps[create_edit_fyp::get_fyp_id()]["moderator"] = (search_field.length() == 0 ? admins : visible_admins).keys()[selected_option - 1];
                update_fyps();
                return_page();
                break;
            }
            case static_cast<int>(Key::ESCAPE): {
                return_page();
                break;
            }
        }
    }
}