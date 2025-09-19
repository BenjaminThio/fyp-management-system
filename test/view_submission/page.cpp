#include <sstream>
#include <conio.h>
#include <array>
#include <filesystem>
#include "keyboard.h"
#include "globals.h"
#include "renderer.h"
#include "input.h"
#include "database.h"
#include "search.h"
#include "grading/page.h"
#include "terminal.h"
using namespace std;

namespace fs = filesystem;

namespace view_submission {
    static json submitters = json::dictionary{};
    static json visible_submitters = json::dictionary{};
    static int selected_option = 0;
    static string search_field;
    static int local_caret_pos;
    static int input_field_view_offset;
    static int visible_quantity = 30;
    static int field_length = 30;
    static int selection_offset = 0;

    json get_submitter() {
        return (search_field.length() == 0 ? submitters : visible_submitters)[(search_field.length() == 0 ? submitters : visible_submitters).keys()[selection_offset + selected_option - 1]];
    }

    void refresh_submitters() {
        /*
        for (auto& [key, val] : fyps.as_dictionary()) {
            if ()
                owned_fyps[key] = val;
            
            if (val["wishlistApproved"].size() == 0) continue;

            for (json& email : val["wishlistApproved"].as_list()) {
                fs::path destination = DESTINATION / fs::path(email.as_string());

                if (fs::exists(destination) && !fs::is_empty(destination)) {
                    submitters[email.as_string()] = get_users()[email.as_string()];
                    if (!submitters[email.as_string()].contains(json("fyp_1_id"))) {
                        submitters[email.as_string()]["fyp_1_id"] = json(key);
                    } else if (!submitters[email.as_string()].contains(json("fyp_2_id"))) {
                        submitters[email.as_string()]["fyp_2_id"] = json(key);
                    } else {
                        continue;
                    }
                }
            }
        }
        */
        for (auto& [key, val] : fyps.as_dictionary()) {
            for (auto& file : get_files(DESTINATION)) {
                fs::path email = file.filename();

                if (!submitters.contains(json(email.string())) && val["ownerUUID"].as_string() == user["info"]["email"].as_string() && val["wishlistApproved"].contains(json(file.filename().string()))
                    ||
                    !submitters.contains(json(email.string())) && val.contains(json("moderator")) && val["moderator"].as_string() == user["info"]["email"].as_string() && val["wishlistApproved"].contains(json(file.filename().string()))
                ) {
                    vector<fs::path> approved_fyp_ids = get_files(DESTINATION / email);

                    submitters[email.string()] = get_users()[email.string()];

                    if (approved_fyp_ids.size() > 0 && !submitters[email.string()].contains(json("fyp_1_id"))) {
                        submitters[email.string()]["fyp_1_id"] = json(approved_fyp_ids[0].filename().string());
                    }
                    if (approved_fyp_ids.size() > 1 && !submitters[email.string()].contains(json("fyp_2_id"))) {
                        submitters[email.string()]["fyp_2_id"] = json(approved_fyp_ids[1].filename().string());
                    }
                    break;
                }
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
        renderer << "Search: " << render_input_field(search_field, local_caret_pos, input_field_view_offset, field_length, (selected_option == 0 ? BG_GREEN : BG_WHITE)) << endl << endl;

        if (search_field.length() > 0) {
            visible_submitters = json::dictionary{};
            
            for (auto& [key, val] : submitters.as_dictionary()) {
                if (search::similar(val["info"]["username"].as_string() + " " + val["info"]["email"].as_string(), search_field)) {
                    visible_submitters[key] = val;
                }
            }
        }

        size_t counter = 0;

        for (size_t i = selection_offset; i < selection_offset + (visible_quantity > (search_field.length() == 0 ? submitters : visible_submitters).size() ? (search_field.length() == 0 ? submitters : visible_submitters).size() : visible_quantity); i++, counter++) {
            renderer << ((counter + 1 == selected_option) ? '>' : ' ') << ' ' << i + 1 << ". " << (search_field.length() == 0 ? submitters : visible_submitters)[(search_field.length() == 0 ? submitters : visible_submitters).keys()[i]]["info"]["username"].as_string() << endl;
        }

        /*
        for (auto& [key, val] : (search_field.length() == 0 ? submitters : visible_submitters).as_dictionary()) {
            if (counter < visible_quantity) {
                renderer << ((counter++ + 1 == selected_option) ? '>' : ' ') << ' ' << val["info"]["username"].as_string() << endl;
            }
        }
        */

        switch (selected_option) {
            case 0:
                manual_cursor_input_pos = { local_caret_pos + 8, 6 };
                break;
        }
        caret_handler();
    }

    static void up() {
        if (selected_option - 1 > 0) {
            selected_option--;
        } else {
            if (selected_option > 0) {
                if (selection_offset - 1 >= 0) {
                    selection_offset--;
                } else {
                    if (selected_option - 1 >= 0) {
                        selected_option--;
                    } else {
                        // selected_option = (visible_quantity > (search_field.length() == 0 ? submitters : visible_submitters).size() ? (search_field.length() == 0 ? submitters : visible_submitters).size() : visible_quantity); // submitters.size() - 1
                    }
                }
            } else {
                // selected_option = (visible_quantity > (search_field.length() == 0 ? submitters : visible_submitters).size() ? (search_field.length() == 0 ? submitters : visible_submitters).size() : visible_quantity);
            }
        }
        /*
        if (selected_option - 1 >= 0) {
            selected_option--;
        } else {
            selected_option = (visible_quantity > (search_field.length() == 0 ? submitters : visible_submitters).size() ? (search_field.length() == 0 ? submitters : visible_submitters).size() : visible_quantity);
        }
        */
    }

    static void down() {
        if (selected_option + 1 <= (visible_quantity > (search_field.length() == 0 ? submitters : visible_submitters).size() ? (search_field.length() == 0 ? submitters : visible_submitters).size() : visible_quantity)) { // selected_option + 1 < submitters.size()
            selected_option++;
        } else {
            if (selection_offset + selected_option + 1 <= (search_field.length() == 0 ? submitters : visible_submitters).size()) {
                selection_offset++;
            } else {
                // selected_option = 0;
            }
        }
        /*
        if (selected_option + 1 <= (visible_quantity > (search_field.length() == 0 ? submitters : visible_submitters).size() ? (search_field.length() == 0 ? submitters : visible_submitters).size() : visible_quantity)) {
            selected_option++;
        } else {
            selected_option = 0;
        }
        */
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
                up();
                // play_sound("squeak");
                render_page();
                caret_handler();
                break;
            }
            case static_cast<int>(Key::DOWN): {
                down();
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
                grading::start();
                grading::field_coord_correction();
                redirect(static_cast<int>(Page::GRADING));
                break;
            }
            case static_cast<int>(Key::ESCAPE): {
                // Data reset
                submitters = json::dictionary{};
                visible_submitters = json::dictionary{};
                selected_option = 0;
                search_field = "";
                local_caret_pos = 0;
                input_field_view_offset = 0;
                selection_offset = 0;

                return_page();
                break;
            }
        }
    }
}