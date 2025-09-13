#include <optional>
#include <sstream>
#include <iostream>
#include <conio.h>
#include <map>
#include <array>
#include <cstring>
#include "magic_enum.hpp"
#include "home/page.h"
#include "globals.h"
#include "keyboard.h"
#include "renderer.h"
#include "audio.h"
#include "terminal.h"
#include "dialog_box.h"

using namespace std;
using namespace magic_enum;

namespace home {
    static array<const char*, 2> hint_messages = { "* Press the [Up] and [Down] arrow keys to navigate the selections.", "* Press [Enter] to select." };
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
        if (!is_authorized()) {
            renderer
            << frame_chunk << endl << endl
            << hint_messages[0] << endl << hint_messages[1] << endl << endl
            << (selected_option == static_cast<int>(UnauthorizedOption::SIGN_UP) ? '>' : ' ') << " SIGN UP" << endl
            << (selected_option == static_cast<int>(UnauthorizedOption::LOGIN) ? '>' : ' ') << " SIGN IN" << endl
            << (selected_option == static_cast<int>(UnauthorizedOption::EXIT) ? '>' : ' ') << " EXIT";
        } else {
            string welcome_message = "Welcome " + user["info"]["username"].parse_string(0, false, true) + "!";

            if (get_role() == Role::STUDENT) {
                renderer
                << frame_chunk << endl << endl
                << welcome_message << string(strlen(hint_messages[0]) - welcome_message.size() - strlen(" Role: Student") - 1, ' ') + " Role: Student" << endl << endl
                << hint_messages[0] << endl << hint_messages[1] << endl << endl
                << (selected_option == static_cast<int>(StudentOption::FYP_LIST) ? '>' : ' ') << " FINAL YEAR PROJECTS LIST" << endl
                << (selected_option == static_cast<int>(StudentOption::WISHLIST) ? '>' : ' ') << " SHORTLIST" << endl
                << (selected_option == static_cast<int>(StudentOption::SUBMIT) ? '>' : ' ') << " SUBMIT" << endl
                << (selected_option == static_cast<int>(StudentOption::LOG_OUT) ? '>' : ' ') << " LOGOUT" << endl
                << (selected_option == static_cast<int>(StudentOption::EXIT) ? '>' : ' ') << " EXIT";
            } else if (get_role() == Role::ADMIN) {
                renderer
                << frame_chunk << endl << endl
                << welcome_message << string(strlen(hint_messages[0]) - welcome_message.size() - strlen(" Role: Admin") - 1, ' ') + " Role: Admin" << endl << endl
                << hint_messages[0] << endl << hint_messages[1] << endl << endl
                << (selected_option == static_cast<int>(AdminOption::FYP_LIST) ? '>' : ' ') << " FINAL YEAR PROJECTS LIST" << endl
                << (selected_option == static_cast<int>(AdminOption::CONSOLE) ? '>' : ' ') << " CONSOLE" << endl
                << (selected_option == static_cast<int>(AdminOption::GRADING) ? '>' : ' ') << " GRADING" << endl
                << (selected_option == static_cast<int>(AdminOption::LOG_OUT) ? '>' : ' ') << " LOG OUT" << endl
                << (selected_option == static_cast<int>(AdminOption::EXIT) ? '>' : ' ') << " EXIT";
            }
        }
    }

    /*
    static void push_frame_t(ostringstream& renderer) {
        renderer << "Welcome " << user["info"]["username"].parse_string(0, false, true) << '!' << string(10, ' ') + " Role: " << enum_name<Role>(get_role()) << endl;
        for (int option_index = 0; option_index < enum_count<home::Option>(); option_index++) {
            optional<home::Option> option = enum_cast<home::Option>(option_index);

            if (option.has_value()) renderer << ((option_index == selected_option) ? '>' : ' ') << ' ' << enum_name<home::Option>(option.value()) << endl;
            else throw runtime_error("Option not found!");
        }
        renderer << endl << frame_chunk;
    }
    */

    void keyboard_input_callback() {
        if (_kbhit()) {
            int key = _getch();

            switch (key) {
                case 224: {
                    int special_key = _getch();

                    switch (special_key) {
                        case static_cast<int>(Key::UP): {
                            if (selected_option - 1 >= 0) selected_option--;
                            else {
                                if (!is_authorized()) {
                                    selected_option = enum_count<UnauthorizedOption>() - 1;
                                } else if (get_role() == Role::STUDENT) {
                                    selected_option = enum_count<StudentOption>() - 1;
                                } else if (get_role() == Role::ADMIN) {
                                    selected_option = enum_count<AdminOption>() - 1;
                                }
                            };

                            render_page();
                            play_sound("squeak");
                            break;
                        }
                        case static_cast<int>(Key::DOWN): {
                            //if (selected_option + 1 < enum_count<home::Option>()) selected_option++;

                            if (!is_authorized()) {
                                if (selected_option + 1 < enum_count<UnauthorizedOption>())
                                    selected_option++;
                                else 
                                    selected_option = 0;
                            } else {
                                if (get_role() == Role::STUDENT) {
                                    if (selected_option + 1 < enum_count<StudentOption>())
                                        selected_option++;
                                    else
                                        selected_option = 0;
                                }else if (get_role() == Role::ADMIN) {
                                    if (selected_option + 1 < enum_count<AdminOption>())
                                        selected_option++;
                                    else
                                        selected_option = 0;
                                }
                            }

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

                    if (!is_authorized()) {
                        switch (selected_option) {
                            case static_cast<int>(UnauthorizedOption::SIGN_UP):
                                redirect(static_cast<int>(Page::SIGN_UP), "select", true);
                                break;
                            case static_cast<int>(UnauthorizedOption::LOGIN):
                                redirect(static_cast<int>(Page::LOGIN), "select", true);
                                break;
                            case static_cast<int>(UnauthorizedOption::EXIT):
                                exit(0);
                        }
                    } else {
                        if (get_role() == Role::STUDENT) {
                            switch (selected_option) {
                                case static_cast<int>(StudentOption::FYP_LIST):
                                    redirect(static_cast<int>(Page::FYP_LIST), "select", true);
                                    break;
                                case static_cast<int>(StudentOption::WISHLIST):
                                    redirect(static_cast<int>(Page::WISHLIST), "select", true);
                                    break;
                                case static_cast<int>(StudentOption::SUBMIT):
                                    dialog_box();
                                    break;
                                case static_cast<int>(StudentOption::LOG_OUT):
                                    selected_option = 0;
                                    clear_session();
                                    break;
                                case static_cast<int>(StudentOption::EXIT):
                                    exit(0);
                            }
                        } else if (get_role() == Role::ADMIN) {
                            switch (selected_option) {
                                case static_cast<int>(AdminOption::FYP_LIST):
                                    redirect(static_cast<int>(Page::FYP_LIST), "select", true);
                                    break;
                                case static_cast<int>(AdminOption::CONSOLE):
                                    redirect(static_cast<int>(Page::CONSOLE), "select", true);
                                    break;
                                case static_cast<int>(AdminOption::GRADING):
                                    redirect(static_cast<int>(Page::GRADING), "select", true);
                                    break;
                                case static_cast<int>(AdminOption::LOG_OUT):
                                    selected_option = 0;
                                    clear_session();
                                    break;
                                case static_cast<int>(StudentOption::EXIT):
                                    exit(0);
                            }
                        }
                    }
                    /*
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
                        case static_cast<int>(home::Option::GRADING): {
                            redirect(static_cast<int>(Page::GRADING));
                            break;
                        }
                        case static_cast<int>(home::Option::SUBMIT): {
                            dialog_box();
                            break;
                        }
                        case static_cast<int>(home::Option::TEST): {
                            warning_message("Warning!", "TESTING123.");
                            break;
                        }
                        case static_cast<int>(home::Option::EXIT): {
                            exit(0);
                            break;
                        }
                    }
                    */
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