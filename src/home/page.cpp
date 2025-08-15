#include <optional>
#include <sstream>
#include <iostream>
#include <conio.h>
#include "magic_enum.hpp"
#include "home/page.h"
#include "globals.h"
#include "keyboard.h"
#include "renderer.h"
#include "audio.h"
using namespace std;
using namespace magic_enum;

namespace home {
    static int selected_option = 0;

    void push_frame(ostringstream& renderer) {
        for (int option_index = 0; option_index < enum_count<home::Option>(); option_index++) {
            optional<home::Option> option = enum_cast<home::Option>(option_index);

            if (option.has_value()) renderer << ((option_index == selected_option) ? '>' : ' ') << ' ' << enum_name<home::Option>(option.value()) << endl;
            else throw runtime_error("Option not found!");
        }
    }

    void keyboard_input_callback() {
        if (_kbhit()) {
            int key = _getch();

            switch (key) {
                case 224: {
                    int special_key = _getch();

                    switch (special_key) {
                        case static_cast<int>(Key::UP): {
                            if (selected_option - 1 >= 0) selected_option--;
                            else selected_option = enum_count<home::Option>() - 1;

                            render_page();
                            play_sound("squeak");
                            break;
                        }
                        case static_cast<int>(Key::DOWN): {
                            if (selected_option + 1 < enum_count<home::Option>()) selected_option++;
                            else selected_option = 0;

                            render_page();
                            play_sound("squeak");
                            break;
                        }
                    }

                    break;
                }
                case static_cast<int>(Key::ENTER): {
                    play_sound("select");

                    switch (selected_option) {
                        case static_cast<int>(home::Option::FYP_LIST): {
                            page = static_cast<int>(Page::FYP_LIST);
                            break;
                        }
                        case static_cast<int>(home::Option::CONSOLE): {
                            page = static_cast<int>(Page::CONSOLE);
                            break;
                        }
                        case static_cast<int>(home::Option::ACCOUNT): {
                            page = static_cast<int>(Page::SIGN_UP);
                            break;
                        }
                        case static_cast<int>(home::Option::EXIT): {
                            exit(0);
                            break;
                        }
                    }

                    render_page();
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