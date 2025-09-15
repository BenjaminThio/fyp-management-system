#include <sstream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <array>
#include "keyboard.h"
#include "globals.h"
#include "renderer.h"
#include "audio.h"
#include "table.h"
#include "text_formatter.h"
using namespace std;
using namespace ansi;

namespace wishlist_approval {
    static string id;
    static coord_2d selected_coord = { 0, 0 };
    static array<json::list, 2> wishlist = { json::list{}, json::list{} };

    void refresh_wishlist_data() {
        wishlist[0] = fyps[id]["wishlistPending"].as_list();
        wishlist[1] = fyps[id]["wishlistApproved"].as_list();
    }

    void wishlist_approval(const string& fyp_id) {
        id = fyp_id;
        refresh_wishlist_data();
        redirect(static_cast<int>(Page::WISHLIST_APPROVAL));
    }

    void push_frame(ostringstream& renderer) {
        // cout << fyps[id]["pendingWishlist"].parse_string();
        
        vector<vector<string>> pending_emails;
        vector<vector<string>> approved_emails;

        for (size_t i = 0; i < wishlist[0].size(); i++) {
            pending_emails.push_back({ selected_coord.x == 0 && selected_coord.y == i ? format(wishlist[0][i].as_string(), FG_BLACK, BG_GREEN) : format(wishlist[0][i].as_string(), FG_YELLOW) });
        }
        for (size_t i = 0; i < wishlist[1].size(); i++) {
            approved_emails.push_back({ selected_coord.x == 1 && selected_coord.y == i ? format(wishlist[1][i].as_string(), FG_BLACK, BG_YELLOW) : format(wishlist[1][i].as_string(), FG_GREEN) });
        }

        renderer << "Title: " << fyps[id]["info"]["name"].as_string()
        << endl << "ID: " << id << endl
        << endl << generate_table({ { "Pending", "Approved" }, { 
            (wishlist[0].size() > 0 ? generate_table(pending_emails) : R"(Noone wishlist this FYP currently ¯\(ツ)/¯)"), 
            (wishlist[1].size() > 0 ? generate_table(approved_emails) : R"(Noone is assigned to this FYP currently ¯\(ツ)/¯)")
        } }) << endl
        << (selected_coord.y == wishlist[selected_coord.x].size() ? format(string("SAVE"), UNDERLINE) : "SAVE");
    }

    void keyboard_input_callback() {
        if (_kbhit()) {
            int key = _getch();

            switch (key) {
                case 0:
                case 224: {
                    int special_key = _getch();

                    switch(special_key) {
                        case static_cast<int>(Key::UP):
                            if (selected_coord.y - 1 >= 0) {
                                selected_coord.y--;
                            } else {
                                selected_coord.y = wishlist[selected_coord.x].size(); // - 1;
                            }
                            break;
                        case static_cast<int>(Key::DOWN):
                            if (selected_coord.y + 1 <= wishlist[selected_coord.x].size()) {
                                selected_coord.y++;
                            } else {
                                selected_coord.y = 0;
                            }
                            break;
                        case static_cast<int>(Key::LEFT):
                            if (selected_coord.y == wishlist[selected_coord.x].size()) return;

                            if (selected_coord.x - 1 >= 0 && wishlist[selected_coord.x - 1].size() > 0) {
                                selected_coord.x--;
                            } else {
                                if (wishlist[wishlist.size() - 1].size() > 0)
                                    selected_coord.x = wishlist.size() - 1;
                            }
                            if (selected_coord.y + 1 > wishlist[selected_coord.x].size()) {
                                selected_coord.y = wishlist[selected_coord.x].size() - 1;
                            }
                            break;
                        case static_cast<int>(Key::RIGHT):
                            if (selected_coord.y == wishlist[selected_coord.x].size()) return;

                            if (selected_coord.x + 1 < wishlist.size() && wishlist[selected_coord.x + 1].size() > 0) {
                                selected_coord.x++;
                            } else {
                                if (wishlist[0].size() > 0)
                                    selected_coord.x = 0;
                            }
                            if (selected_coord.y + 1 > wishlist[selected_coord.x].size()) {
                                selected_coord.y = wishlist[selected_coord.x].size() - 1;
                            }
                            break;
                    }
                    render_page();
                    break;
                }
                case static_cast<int>(Key::ENTER): {
                    if (selected_coord.y == wishlist[selected_coord.x].size()) {
                        fyps[id]["wishlistPending"] = wishlist[0];
                        fyps[id]["wishlistApproved"] = wishlist[1];

                        update_fyps();
                        return_page();
                        return;
                    }

                    string selected_email = wishlist[selected_coord.x][selected_coord.y].as_string();

                    wishlist[selected_coord.x].erase(wishlist[selected_coord.x].begin() + selected_coord.y);
                    wishlist[static_cast<int>(!static_cast<bool>(selected_coord.x))].push_back(json(selected_email));
                    if (wishlist[selected_coord.x].size() > 0) {
                        if (selected_coord.y + 1 > wishlist[selected_coord.x].size()) {
                            selected_coord.y = wishlist[selected_coord.x].size() - 1;
                        }
                    } else {
                        selected_coord.x = static_cast<int>(!static_cast<bool>(selected_coord.x));
                        selected_coord.y = wishlist[selected_coord.x].size() - 1;
                    }
                    
                    render_page();
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