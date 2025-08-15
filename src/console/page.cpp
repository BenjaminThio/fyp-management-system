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
using namespace std;
using namespace ansi;

namespace console {
    static int selected_row = 0;
    static int selected_option = 0;
    json j;

    void start() {
        j = load("../data/test.json");
    }

    void push_frame(ostringstream& renderer) {
        vector<vector<string>> table = {
            { "No.", "Titles", "Public" }
        };
        
        for (int i = 0; i < j.size(); i++) {
            vector<string> keys = j.keys();
            vector<string> row = {
                to_string(i + 1) + ".",
                keys[i],
                ((selected_row == i && selected_option == 0) ? format("ON", FG_BLACK, BG_GREEN) : format("ON", FG_GREEN)),
                ((selected_row == i && selected_option == 1) ? format("View", FG_BLACK, BG_GREEN) : format("View", FG_GREEN)),
                ((selected_row == i && selected_option == 2) ? format("Edit", FG_BLACK, BG_YELLOW) : format("Edit", FG_YELLOW)),
                ((selected_row == i && selected_option == 3) ? format("Delete", FG_BLACK, BG_RED) : format("Delete", FG_RED))
            };

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
                            else selected_row = j.size() - 1;
                            break;
                        }
                        case static_cast<int>(Key::DOWN): {
                            if (selected_row + 1 < j.size()) selected_row++;
                            else selected_row = 0;
                            break;
                        }
                        case static_cast<int>(Key::LEFT): {
                            if (selected_option - 1 >= 0) selected_option--;
                            else selected_option = 3;
                            break;
                        }
                        case static_cast<int>(Key::RIGHT): {
                            if (selected_option + 1 <= 3) selected_option++;
                            else selected_option = 0;
                            break;
                        }
                    }

                    render_page();
                    play_sound("select");
                    break;
                }
                case static_cast<int>(Key::ESCAPE): {
                    page = previous_page;

                    render_page();
                    play_sound("squeak");
                    break;
                }
            }
        }
    }
}