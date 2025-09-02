#include <sstream>
#include <conio.h>
#include "keyboard.h"
#include "globals.h"
#include "renderer.h"
#include "audio.h"
#include "table.h"
#include "json.h"
#include "database.h"
using namespace std;

namespace wishlist {
    void push_frame(ostringstream& renderer) {
        vector<vector<string>> wishlist_table = { { "Wishlist Titles" } };

        for (json& id : get_wishlist())
            wishlist_table.push_back({ "⭐️ " + fyps[id.parse_string(0, false, true)]["info"]["name"].parse_string(0, false, true) });
        renderer << generate_table(wishlist_table) << endl;
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