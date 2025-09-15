#include <string>
#include <vector>
#include <conio.h>
#include <cmath>
#include <cstring>
#include <array>
#include "json.h"
#include "keyboard.h"
#include "globals.h"
#include "console/page.h"
#include "database.h"
#include "table.h"
#include "renderer.h"
#include "audio.h"
#include "text_formatter.h"
#include "input.h"
#include "utils.h"
#include "magic_enum.hpp"
#include "buffer.h"
#include "fyp/page.h"
#include "create_edit/page.h"
#include "wishlist_approval/page.h"
using namespace std;
using namespace ansi;
using namespace magic_enum;

namespace console {
    static const char* DELETE_LABEL = "DELETE";
    static const char* WISHLIST_APPROVAL_LABEL = "Shortlist Approval";
    static const int PADDING = 20;
    // static int selected_row = 0;
    // static int selected_option = 0;
    static coord_2d selected_coord;
    static json::dictionary authorized_fyps;
    static vector<json::dictionary> grouped_fyps;
    static int group_idx = 0;
    static int direction = 0;

    // Select
    static const array<string, 6> select_options = { "Show All", "5", "10", "20", "50", "100" };
    static bool collapse = false;
    static int selected_option = static_cast<int>(SelectOption::TEN);

    static int get_group_amount() {
        switch (selected_option) {
            case static_cast<int>(SelectOption::SHOW_ALL):
                return authorized_fyps.size();
            case static_cast<int>(SelectOption::FIVE):
                return 5;
            case static_cast<int>(SelectOption::TEN):
                return 10;
            case static_cast<int>(SelectOption::TWENTY):
                return 20;
            case static_cast<int>(SelectOption::FIFTY):
                return 50;
            case static_cast<int>(SelectOption::ONE_HUNDRED):
                return 100;
            default:
                throw runtime_error("Unknown option.");
        }
    }

    static void update_grouping() {
        grouped_fyps = group_dict(authorized_fyps, get_group_amount());
    }

    void refresh_fyps_data() {
        authorized_fyps = get_fyps();
        update_grouping();
    }

    void push_frame(ostringstream& renderer) {
        if (get_role() == Role::STUDENT) {
            renderer << "Only admins can view the console page." << endl;
            return;
        }

        if (authorized_fyps.size() == 0) {
            renderer << R"(Your console is empty ¯\(ツ)/¯)" << endl << "Press [Enter] to create your first FYP.";
            return;
        }

        vector<vector<string>> table = {
            { "No.", "Titles", "ID", "Public" }
        };
        size_t i = 1;
        
        for (auto& [key, val] : grouped_fyps[group_idx]) {
            vector<string> row = {
                to_string(i) + ".",
                render_static_input_field(val["info"]["name"].parse_string(0, false, true), 40, BG_BLACK, FG_WHITE),
                key,
                collapse ? (val["isPublic"] ? "ON" : "OFF") : (
                    selected_coord.y == i && selected_coord.x == 0
                    ?
                    (val["isPublic"] ? format("ON", FG_BLACK, BG_GREEN) : format("OFF", FG_BLACK, BG_RED))
                    :
                    (val["isPublic"] ? format("ON", FG_GREEN) : format("OFF", FG_RED))
                ),
                (collapse ? "View" : (selected_coord.y == i && selected_coord.x == 1) ? format("View", FG_BLACK, BG_GREEN) : format("View", FG_GREEN)),
                (collapse ? "Edit" : (selected_coord.y == i && selected_coord.x == 2) ? format("Edit", FG_BLACK, BG_YELLOW) : format("Edit", FG_YELLOW)),
                (collapse ? (WISHLIST_APPROVAL_LABEL + get_pending_wishlist_amount(key)) : (selected_coord.y == i && selected_coord.x == 3) ? format((WISHLIST_APPROVAL_LABEL + get_pending_wishlist_amount(key)), FG_BLACK, BG_LIGHT_RED) : format((WISHLIST_APPROVAL_LABEL + get_pending_wishlist_amount(key)), FG_LIGHT_RED)),
                (collapse ? DELETE_LABEL : (selected_coord.y == i && selected_coord.x == 4) ? format(DELETE_LABEL, FG_BLACK, BG_RED) : format(DELETE_LABEL, FG_RED))
            };
            i++;
            table.push_back(row);
        }

        string front_buffer = generate_table(table);
        size_t buffer_length = split(front_buffer, '\n')[0].length();
        front_buffer.insert(0, (selected_coord.y == 0 && direction == static_cast<int>(Direction::LEFT) ? format("Add Fyp", UNDERLINE) : "Add Fyp") + string(buffer_length - strlen("Add Fyp"), ' ') + "\n");
        front_buffer.insert(0, string(buffer_length, ' ') + "\n");

        vector<vector<string>> select_table = { { (
            selected_coord.y == 0 && direction == static_cast<int>(Direction::RIGHT)
            ?
            format(select_options[selected_option], UNDERLINE)
            :
            select_options[selected_option]) + string(strlen(WISHLIST_APPROVAL_LABEL) - select_options[selected_option].size() - 4, ' ') } };

        if (collapse) {
            for (size_t i = 0; i < select_options.size(); i++) {
                select_table.push_back({
                    format(select_options[i] + string(strlen(WISHLIST_APPROVAL_LABEL) - select_options[i].size() - 4, ' '), 
                    (i == selected_option) ?
                    BG_GREEN
                    :
                    BG_RED
                    , FG_BLACK)
                });
            }
        }

        string back_buffer = generate_table(select_table);

        renderer << overlay(front_buffer, back_buffer, buffer_length - strlen(WISHLIST_APPROVAL_LABEL) - strlen(DELETE_LABEL) + 5)
        << endl << string(PADDING, ' ')
        << (selected_coord.y == grouped_fyps[group_idx].size() + 1 && direction == static_cast<int>(Direction::LEFT) ? format("<", UNDERLINE) : "<")
        << string(buffer_length - (PADDING * 2) - 2, ' ')
        << (selected_coord.y == grouped_fyps[group_idx].size() + 1 && direction == static_cast<int>(Direction::RIGHT) ? format(">", UNDERLINE) : ">");
    }

    void keyboard_input_callback() {
        if (_kbhit()) {
            int key = _getch();

            switch (key) {
                case 0:
                case 224: {
                    int special_key = _getch();

                    switch (special_key) {
                        case static_cast<int>(Key::UP): {
                            if (collapse) {
                                if (selected_option - 1 >= 0) {
                                    selected_option--;
                                } else {
                                    selected_option = enum_count<SelectOption>() - 1;
                                }
                            } else {
                                if (selected_coord.y - 1 >= 0) selected_coord.y--;
                                else {
                                    selected_coord.y = grouped_fyps[group_idx].size() + 1;
                                    direction = round(selected_coord.x / 5.0);
                                }
                            }
                            break;
                        }
                        case static_cast<int>(Key::DOWN): {
                            if (collapse) {
                                if (selected_option + 1 < enum_count<SelectOption>()) {
                                    selected_option++;
                                } else {
                                    selected_option = 0;
                                }
                            } else {
                                if (selected_coord.y + 1 <= grouped_fyps[group_idx].size() + 1) {
                                    selected_coord.y++;
                                    if (selected_coord.y == grouped_fyps[group_idx].size() + 1)
                                        direction = round(selected_coord.x / 5.0);
                                }
                                else
                                    selected_coord.y = 0;
                            }
                            break;
                        }
                        case static_cast<int>(Key::LEFT): {
                            if (collapse) return;

                            if (selected_coord.y == 0 || selected_coord.y == grouped_fyps[group_idx].size() + 1) {
                                if (direction - 1 >= 0)
                                    direction--;
                                else
                                    direction = enum_count<Direction>() - 1;
                            } else {
                                if (selected_coord.x - 1 >= 0)
                                    selected_coord.x--;
                                else
                                    selected_coord.x = 4;
                            }
                            break;
                        }
                        case static_cast<int>(Key::RIGHT): {
                            if (collapse) return;

                            if (selected_coord.y == 0 || selected_coord.y == grouped_fyps[group_idx].size() + 1) {
                                if (direction + 1 < enum_count<Direction>())
                                    direction++;
                                else
                                    direction = 0;
                            }
                            else {
                                if (selected_coord.x + 1 <= 4)
                                    selected_coord.x++;
                                else
                                    selected_coord.x = 0;
                            }
                            break;
                        }
                    }

                    render_page();
                    play_sound("select");
                    break;
                }
                case static_cast<int>(Key::ENTER): {
                    if (selected_coord.y == 0) {
                        switch (direction) {
                            case static_cast<int>(Direction::LEFT):
                                redirect(static_cast<int>(Page::CREATE_EDIT));
                                break;
                            case static_cast<int>(Direction::RIGHT):
                                collapse = !collapse;

                                if (!collapse) update_grouping();

                                render_page();
                                break;
                        }
                    } else if (selected_coord.y > 0 && selected_coord.y <= grouped_fyps[group_idx].size()) {
                        string fyp_id = json(grouped_fyps[group_idx]).keys()[selected_coord.y - 1];
                        json& fyp = fyps[fyp_id];

                        switch (selected_coord.x) {
                            case static_cast<int>(Option::PUBLIC):
                                fyp["isPublic"] = !fyp["isPublic"];
                                save("../data/fyp.json", fyps);
                                refresh_fyps_data();
                                break;
                            case static_cast<int>(Option::VIEW):
                                fyp::view(fyp_id);
                                break;
                            case static_cast<int>(Option::EDIT):
                                create_edit_fyp::edit(fyp_id);
                                break;
                            case static_cast<int>(Option::WISHLIST_APPROVAL):
                                wishlist_approval::wishlist_approval(fyp_id);
                                break;
                            case static_cast<int>(Option::DELETEE):
                                break;
                        }

                        render_page();
                    } else if (selected_coord.y == grouped_fyps[group_idx].size() + 1) {
                        switch (direction) {
                            case static_cast<int>(Direction::LEFT):
                                if (group_idx - 1 > 0)
                                    group_idx--;
                                else
                                    group_idx = grouped_fyps.size() - 1;
                                selected_coord.y = grouped_fyps[group_idx].size() + 1; 
                                
                                render_page();
                                break;
                            case static_cast<int>(Direction::RIGHT):
                                if (group_idx + 1 < grouped_fyps.size())
                                    group_idx++;
                                else
                                    group_idx = 0;
                                selected_coord.y = grouped_fyps[group_idx].size() + 1; 
                                
                                render_page();
                                break;
                        }
                    }
                    break;
                }
                case static_cast<int>(Key::ESCAPE): {
                    return_page();
                    break;
                }
            }
        }
    }
}