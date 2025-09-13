#include <sstream>
#include <conio.h>
#include "keyboard.h"
#include "globals.h"
#include "renderer.h"
#include "audio.h"
#include "table.h"
#include "json.h"
#include "database.h"
#include "text_formatter.h"
using namespace std;
using namespace ansi;

namespace wishlist {
    void push_frame(ostringstream& renderer) {
        if (get_role() == Role::ADMIN) {
            renderer << "Only students can view their wishlist." << endl;
            return;
        }

        if (get_wishlist().size() == 0) {
            renderer << generate_table({ { "Wishlist Titles" }, { R"(Your wishlist is empty ¯\(ツ)/¯)" } });
        } else {
            vector<vector<string>> wishlist_table = { { "Wishlist Titles", "Status" } };

            for (json& id : get_wishlist()) {
                string status;

                switch (get_wishlist_status(id)) {
                    case WishlistStatus::APPROVED:
                        status = "🟢 " + format(string("Approved"), FG_GREEN);
                        break;
                    case WishlistStatus::PENDING:
                        status = "🟡 " + format(string("Pending"), FG_YELLOW);
                        break;
                    case WishlistStatus::REJECTED:
                        status = "🔴 " + format(string("Rejected"), FG_RED);
                        break;
                    case WishlistStatus::DISCARDED:
                        status = "🟣 " + format(string("Discarded"), FG_LIGHT_MAGENTA);
                        break;
                    case WishlistStatus::UNKNOWN:
                    default:
                        status = "🔵 " + format(string("Unknown"), FG_BLUE);
                        break;
                }

                wishlist_table.push_back({ "⭐️ " + fyps[id.parse_string(0, false, true)]["info"]["name"].parse_string(0, false, true), status });
            }
            
            renderer << generate_table(wishlist_table) << endl;
        }
    }

    void keyboard_input_callback() {
        if (_kbhit()) {
            int key = _getch();

            switch (key) {
                case 224: {
                    // int special_key = _getch();
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