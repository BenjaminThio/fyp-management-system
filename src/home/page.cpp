#include <optional>
#include <sstream>
#include <iostream>
#include <conio.h>
#include <map>
#include "magic_enum.hpp"
#include "home/page.h"
#include "globals.h"
#include "keyboard.h"
#include "renderer.h"
#include "audio.h"
#include "terminal.h"

using namespace std;
using namespace magic_enum;

namespace home {
    static int selected_option = 0;
    
    // For typing effect.
    size_t id = 0;
    string frame_chunk;
    bool is_initialized = false;
    map<int, bool> active_tasks;

    static void stop_all_tasks() {
        for (auto& kv : active_tasks) kv.second = false;
    }

    void push_frame(ostringstream& renderer) {
        for (int option_index = 0; option_index < enum_count<home::Option>(); option_index++) {
            optional<home::Option> option = enum_cast<home::Option>(option_index);

            if (option.has_value()) renderer << ((option_index == selected_option) ? '>' : ' ') << ' ' << enum_name<home::Option>(option.value()) << endl;
            else throw runtime_error("Option not found!");
        }
        renderer << endl << frame_chunk;
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
                    stop_all_tasks();
                    frame_chunk.clear();

                    switch (selected_option) {
                        case static_cast<int>(home::Option::FYP_LIST): {
                            redirect(static_cast<int>(Page::FYP_LIST));
                            break;
                        }
                        case static_cast<int>(home::Option::CONSOLE): {
                            redirect(static_cast<int>(Page::CONSOLE));
                            break;
                        }
                        case static_cast<int>(home::Option::ACCOUNT): {
                            redirect(static_cast<int>(Page::SIGN_UP));
                            break;
                        }
                        case static_cast<int>(home::Option::WISHLIST): {
                            redirect(static_cast<int>(Page::WISHLIST));
                            break;
                        }
                        case static_cast<int>(home::Option::CREATE_EDIT): {
                            redirect(static_cast<int>(Page::CREATE_EDIT));
                            break;
                        }
                        case static_cast<int>(home::Option::LOGIN): {
                            redirect(static_cast<int>(Page::LOGIN));
                            break;
                        }
                        case static_cast<int>(home::Option::EXIT): {
                            exit(0);
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