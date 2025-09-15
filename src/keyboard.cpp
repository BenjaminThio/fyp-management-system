#include <thread>
#include <functional>
#include "keyboard.h"
#include "globals.h"
#include "home/page.h"
#include "fyp/page.h"
#include "console/page.h"
#include "fyp_view/page.h"
#include "auth/page.h"
#include "create_edit/page.h"
#include "wishlist/page.h"
#include "grading/page.h"
#include "typing.h"
#include "wishlist_approval/page.h"
#include "submit/page.h"
#include "assign_mod/page.h"

using namespace std;

void detect_keyboard_input() {
    switch (page) {
        case static_cast<int>(Page::HOME): {
            if (!home::is_initialized) {
                int registered_id = home::id;

                home::is_initialized = true;
                home::active_tasks[registered_id] = true;
                home::id++;
                home::frame_chunk.clear();

                thread t(typing, "VIEW, CREATE AND EDIT FYP.", ref(home::frame_chunk), ref(home::active_tasks), registered_id);
                t.detach();
            };
            home::keyboard_input_callback();
            break;
        }
        case static_cast<int>(Page::FYP_LIST):
        case static_cast<int>(Page::CONSOLE):
        case static_cast<int>(Page::FYP_VIEW):
        case static_cast<int>(Page::SIGN_UP):
        case static_cast<int>(Page::LOGIN):
        case static_cast<int>(Page::CREATE_EDIT):
        case static_cast<int>(Page::WISHLIST):
        case static_cast<int>(Page::GRADING):
        case static_cast<int>(Page::WISHLIST_APPROVAL):
        case static_cast<int>(Page::SUBMIT):
        case static_cast<int>(Page::ASSIGN_MOD):
            if (home::is_initialized) home::is_initialized = false;

            switch (page) {
                case static_cast<int>(Page::FYP_LIST): {
                    fyp::keyboard_input_callback();
                    break;
                }
                case static_cast<int>(Page::CONSOLE): {
                    console::keyboard_input_callback();
                    break;
                }
                case static_cast<int>(Page::FYP_VIEW): {
                    fyp_view::keyboard_input_callback();
                    break;
                }
                case static_cast<int>(Page::LOGIN):
                case static_cast<int>(Page::SIGN_UP): {
                    auth::keyboard_input_callback();
                    break;
                }
                case static_cast<int>(Page::CREATE_EDIT): {
                    create_edit_fyp::keyboard_input_callback();
                    break;
                }
                case static_cast<int>(Page::WISHLIST):
                    wishlist::keyboard_input_callback();
                    break;
                case static_cast<int>(Page::GRADING):
                    grading::keyboard_input_callback();
                    break;
                case static_cast<int>(Page::WISHLIST_APPROVAL):
                    wishlist_approval::keyboard_input_callback();
                    break;
                case static_cast<int>(Page::SUBMIT):
                    submit::keyboard_input_callback();
                    break;
                case static_cast<int>(Page::ASSIGN_MOD):
                    assign_mod::keyboard_input_callback();
                    break;
            }
            break;
        default: break;
    }
}