#include "globals.h"
#include "terminal.h"
#include "renderer.h"
#include "audio.h"
#include "auth/page.h"
#include "fyp/page.h"
#include "create_edit/page.h"
#include "database.h"
#include "json.h"
#include <string>
#include <vector>
using namespace std;

string session_id;
int page = static_cast<int>(Page::HOME);
vector<int> previous_page;
json user;
json fyps;

void start() {
    json j = load("../data/session.json");

    session_id = j["sessionId"].parse_string(0, false, true);
    user = get_user();
    fyps = load("../data/fyp.json");
}

json get_user() {
    json users = load("../data/user.json");

    return users[session_id];
}

json::list get_wishlist() {
    return user["wishlist"].as_list();
}

json::dictionary get_fyps() {
    json::dictionary authorized_fyps;

    for (auto& [key, val] : fyps.as_dictionary()) {
        if (val["ownerUUID"].parse_string(0, false, true) == session_id) {
            authorized_fyps[key] = val;
        }
    }

    return authorized_fyps;
}

void redirect(int p, const string& snd, const bool incognito_mode) {
    play_sound(snd);
    if (!incognito_mode)
        previous_page.push_back(page);
    page = static_cast<int>(p);

    switch (p) {
        case static_cast<int>(Page::CONSOLE):
        case static_cast<int>(Page::HOME):
        case static_cast<int>(Page::NOT_FOUND):
        case static_cast<int>(Page::WISHLIST):
            terminal::hide_cursor();
            break;
        case static_cast<int>(Page::LOGIN):
        case static_cast<int>(Page::SIGN_UP):
            auth::caret_handler();
            break;
        case static_cast<int>(Page::FYP_LIST):
            fyp::caret_handler();
            break;
        case static_cast<int>(Page::CREATE_EDIT):
            create_edit_fyp::caret_handler();
            break;
    }

    render_page();
}

void return_page() {
    redirect(previous_page[previous_page.size() - 1], "squeak", true);
    previous_page.pop_back();
}