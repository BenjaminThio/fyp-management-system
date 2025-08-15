#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "globals.h"
#include "magic_enum.hpp"
#include "database.h"
#include "uuid_v4.h"
#include "simdjson.h"
#include "table.h"
#include "json.h"
#include "home/page.h"
#include "fyp/page.h"
#include "console/page.h"
#include "auth/page.h"
#include "fyp_view/page.h"
using namespace std;
using namespace magic_enum;
using namespace simdjson;

void render_page() {
    ostringstream renderer;

    system("cls");

    renderer << " ______   _______ \n|  _ \\ \\ / |  ___|\n| |_) \\ V /| |_   \n|  __/ | | |  _|  \n|_|    |_| |_|\n\n";

    switch (page) {
        case static_cast<int>(Page::HOME): {
            home::push_frame(renderer);
            break;
        }
        case static_cast<int>(Page::FYP_LIST): {
            fyp::push_frame(renderer);
            break;
        }
        case static_cast<int>(Page::CONSOLE): {
            console::push_frame(renderer);
            break;
        }
        case static_cast<int>(Page::SIGN_UP): {
            sign_up::push_frame(renderer);
            break;
        }
        case static_cast<int>(Page::FYP_VIEW): {
            fyp_view::push_frame(renderer);
            break;
        }
        default: {
            optional<Page> scopedPage = enum_cast<Page>(page);

            if (scopedPage.has_value()) {
                renderer << enum_name<Page>(scopedPage.value()) << " page is under developement.";
            } else {
                cerr << "Page not found!" << endl;
                getchar();
                exit(0);
            }
            break;
        }
    }

    cout << renderer.str();
}