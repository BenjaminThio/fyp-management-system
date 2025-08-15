#include "keyboard.h"
#include "globals.h"
#include "home/page.h"
#include "fyp/page.h"
#include "console/page.h"
#include "fyp_view/page.h"

void detect_keyboard_input() {
    switch (page) {
        case static_cast<int>(Page::HOME): {
            home::keyboard_input_callback();
            break;
        }
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
        default: break;
    }
}