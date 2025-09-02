#include <string>
#include <vector>
#include <conio.h>
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
using namespace std;
using namespace ansi;

namespace console {
    static int selected_row = 0;
    static int selected_option = 0;
    static json::dictionary authorized_fyps;

    void start() {
        authorized_fyps = get_fyps();
    }

    void push_frame(ostringstream& renderer) {
        renderer << (selected_row == 0 ? format("Add Fyp", UNDERLINE) : "Add Fyp") << endl;

        vector<vector<string>> table = {
            { "No.", "Titles", "ID", "Public" }
        };
        size_t i = 1;
        
        for (auto& [key, val] : authorized_fyps) {
            vector<string> row = {
                to_string(i + 1) + ".",
                render_static_input_field(val["info"]["name"].parse_string(0, false, true), 40, BG_BLACK, FG_WHITE),
                key,
                ((selected_row == i && selected_option == 0) ? format("ON", FG_BLACK, BG_GREEN) : format("ON", FG_GREEN)),
                ((selected_row == i && selected_option == 1) ? format("View", FG_BLACK, BG_GREEN) : format("View", FG_GREEN)),
                ((selected_row == i && selected_option == 2) ? format("Edit", FG_BLACK, BG_YELLOW) : format("Edit", FG_YELLOW)),
                ((selected_row == i && selected_option == 3) ? format("Wishlist Approval", FG_BLACK, BG_LIGHT_RED) : format("Wishlist Approval", FG_LIGHT_RED)),
                ((selected_row == i && selected_option == 4) ? format("Delete", FG_BLACK, BG_RED) : format("Delete", FG_RED))
            };
            i++;
            table.push_back(row);
        }

        renderer << generate_table(table);
    }

    void keyboard_input_callback() {
        if (_kbhit()) {
            int key = _getch();

            switch (key) {
                case 224: {
                    int special_key = _getch();

                    switch (special_key) {
                        case static_cast<int>(Key::UP): {
                            if (selected_row - 1 >= 0) selected_row--;
                            else selected_row = authorized_fyps.size();
                            break;
                        }
                        case static_cast<int>(Key::DOWN): {
                            if (selected_row + 1 <= authorized_fyps.size()) selected_row++;
                            else selected_row = 0;
                            break;
                        }
                        case static_cast<int>(Key::LEFT): {
                            if (selected_row == 0) return;

                            if (selected_option - 1 >= 0) selected_option--;
                            else selected_option = 4;
                            break;
                        }
                        case static_cast<int>(Key::RIGHT): {
                            if (selected_row == 0) return;

                            if (selected_option + 1 <= 4) selected_option++;
                            else selected_option = 0;
                            break;
                        }
                    }

                    render_page();
                    play_sound("select");
                    break;
                }
                case static_cast<int>(Key::ENTER): {
                    if (selected_row == 0 && selected_option == 0) {
                        redirect(static_cast<int>(Page::CREATE_EDIT));
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