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
#include "dialog_box.h"
using namespace std;
using namespace ansi;

namespace wishlist {
    static int selected_option = 0;
    static json::list wishlist = json::list{};
    
    void update_wishlist() {
        wishlist = get_wishlist();
    }

    void push_frame(ostringstream& renderer) {
        if (get_role() == Role::ADMIN) {
            renderer << "Only students can view their wishlist." << endl;
            return;
        }

        if (wishlist.size() == 0) {
            renderer << generate_table({ { "Shortlist Titles" }, { "Your shortlist is empty." } });
        } else {
            vector<vector<string>> wishlist_table = { { "Shortlist Titles", "Status" } };

            for (size_t i = 0; i < wishlist.size(); i++) {
                string id = wishlist[i].as_string();
                string indicator;
                string status;
                TextFormat bg_color;
                string fyp_name;
                string hint;

                switch (get_wishlist_status(id)) {
                    case WishlistStatus::APPROVED:
                        indicator = (user["wishlist"].contains(id) ? "⭐️ " : "☆  ");
                        status = "🟢 " + format(string("Approved"), FG_GREEN);
                        bg_color = BG_LIGHT_GREEN;
                        fyp_name = fyps[id]["info"]["name"].as_string();
                        hint = "You can't remove an approved FYP.";
                        break;
                    case WishlistStatus::PENDING:
                        indicator = (user["wishlist"].contains(id) ? "⭐️ " : "☆  ");
                        status = "🟡 " + format(string("Pending"), FG_YELLOW);
                        bg_color = BG_LIGHT_YELLOW;
                        fyp_name = fyps[id]["info"]["name"].as_string();
                        hint = "Press [Enter] to unshortlist it.";
                        break;
                    /*
                    case WishlistStatus::REJECTED:
                        status = "🔴 " + format(string("Rejected"), FG_RED);
                        break;
                    */
                    case WishlistStatus::FYP_DELETED:
                        fyp_name = "This FYP not longer exists.";
                        indicator = "❌ ";
                        status = "🔴 " + format(string("Discarded"), FG_RED);
                        // status = "🟣 " + format(string("Discarded"), FG_LIGHT_MAGENTA);
                        hint = "Press [Enter] to delete it.";
                        bg_color = BG_LIGHT_RED;
                        break;
                    case WishlistStatus::DISCARDED:
                        fyp_name = fyps[id]["info"]["name"].as_string();
                        indicator = "❌ ";
                        status = "🔴 " + format(string("Discarded"), FG_RED);
                        // status = "🟣 " + format(string("Discarded"), FG_LIGHT_MAGENTA);
                        hint = "Press [Enter] to delete it.";
                        bg_color = BG_LIGHT_RED;
                        break;
                    case WishlistStatus::UNKNOWN:
                    default:
                        indicator = (user["wishlist"].contains(id) ? "⭐️ " : "☆  ");
                        status = "🟡 " + format(string("Pending"), FG_YELLOW);
                        bg_color = BG_LIGHT_YELLOW;
                        fyp_name = fyps[id]["info"]["name"].as_string();
                        hint = "Press [Enter] to reshortlist it.";
                        break;
                }

                wishlist_table.push_back({ 
                    (selected_option == i
                    ? 
                    indicator + format(fyp_name, FG_BLACK, bg_color) + "\n" +
                    "💡 " + format(hint, FG_BLACK, bg_color)
                    :
                    indicator + fyp_name + "\n "),
                    status
                });
            }
            
            renderer << generate_table(wishlist_table) << endl;
        }
    }

    void up() {
        if (selected_option - 1 >= 0) {
            selected_option--;
        } else {
            selected_option = wishlist.size() - 1;
        }
    }

    void keyboard_input_callback() {
        if (_kbhit()) {
            int key = _getch();

            switch (key) {
                case 0:
                case 224: {
                    int special_key = _getch();

                    switch (special_key) {
                        case static_cast<int>(Key::UP):
                            up();
                            play_sound("squeak");
                            render_page();
                            break;
                        case static_cast<int>(Key::DOWN):
                            if (selected_option + 1 < wishlist.size()) {
                                selected_option++;
                            } else {
                                selected_option = 0;
                            }
                            play_sound("squeak");
                            render_page();
                            break;
                    }
                    break;
                }
                case static_cast<int>(Key::ENTER): {
                    json::list& wishlist_ids = user["wishlist"].as_list();
                    string fyp_id = wishlist[selected_option].as_string();

                    if (get_wishlist_status(json(fyp_id)) == WishlistStatus::DISCARDED || get_wishlist_status(json(fyp_id)) == WishlistStatus::FYP_DELETED) {
                        user["wishlist"].as_list().erase(find(user["wishlist"].as_list().begin(), user["wishlist"].as_list().end(), json(fyp_id)));
                        update_user();
                        update_wishlist();
                        if (wishlist.size() > 0) {
                            if (selected_option >= wishlist.size()) {
                                up();
                            }
                        } else {
                            selected_option = 0;
                        }
                    }
                    else if (user["wishlist"].contains(json(fyp_id))) {
                        json::list& pending_ids = fyps[fyp_id]["wishlistPending"].as_list();
                        json::list& approved_ids = fyps[fyp_id]["wishlistApproved"].as_list();

                        if (fyps[fyp_id]["wishlistPending"].contains(user["info"]["email"])) {
                            wishlist_ids.erase(find(wishlist_ids.begin(), wishlist_ids.end(), json(fyp_id)));
                            pending_ids.erase(find(pending_ids.begin(), pending_ids.end(), user["info"]["email"]));
                        }
                        else if (fyps[fyp_id]["wishlistApproved"].contains(user["info"]["email"])) {
                            play_sound("select");
                            dialog::warning_message("You can't remove an approved FYP.");
                            return;
                            // approved_ids.erase(find(approved_ids.begin(), approved_ids.end(), user["info"]["email"]));
                        }
                    } else {
                        if (user["wishlist"].size() + 1 > MAX_WISHLIST_AMOUNT) {
                            dialog::warning_message("Shortlist limit exceeded! You can only add up to " + to_string(MAX_WISHLIST_AMOUNT) + " projects. Please remove some before adding new ones.");
                            return;
                        }
                        user["wishlist"].as_list().push_back(fyp_id);
                        fyps[fyp_id]["wishlistPending"].as_list().push_back(user["info"]["email"]);
                    }
                    update_user();
                    update_fyps();
                    play_sound("select");
                    render_page();
                    break;
                }
                case static_cast<int>(Key::ESCAPE): {
                    selected_option = 0;
                    wishlist = json::list{};
                    return_page();
                    break;
                }
            }
        }
    }
}