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
vector<int> previous_page = { static_cast<int>(Page::HOME) };
json user;
json fyps;

// sessionId: "benjaminthio@utarstudent.com"

bool is_authorized() {
    json j = load("../data/session.json");

    return !j["sessionId"].is_null();
}

void update_data() {
    json j = load("../data/session.json");

    if (j["sessionId"].is_null()) {
        return;
    }

    session_id = j["sessionId"].parse_string(0, false, true);
    user = get_user();
    fyps = load("../data/fyp.json");
}

void clear_session() {
    json j = load("../data/session.json");

    j["sessionId"] = nullptr;
    user = nullptr;

    save("../data/session.json", j);
    render_page();
}

json get_users() {
    return load("../data/user.json");
}

json get_user() {
    json users = get_users();
    
    return users[session_id];
}

void update_user() {
    json users = get_users();

    users[user["info"]["email"].parse_string(0, false, true)] = user;

    save("../data/user.json", users);
}

void update_fyps() {
    save("../data/fyp.json", fyps);
}

bool is_student(const string& email) {
    return email.ends_with(globals::STUDENT_DOMAIN);
}

bool is_admin(const string& email) {
    return email.ends_with(globals::ADMIN_DOMAIN);
}

Role get_role() {
    string user_email = get_user()["info"]["email"].parse_string(0, false, true);
    
    if (is_student(user_email)) {
        return Role::STUDENT;
    } else if (is_admin(user_email)) {
        return Role::ADMIN;
    } else {
        throw runtime_error("Unknown role!");
    }
}

json::list get_wishlist() {
    return user["wishlist"].as_list();
}

WishlistStatus get_wishlist_status(const json& fyp_id) {
    if (user["wishlist"].contains(fyp_id)) {
        if (fyps[fyp_id.parse_string(0, false, true)]["wishlistPending"].contains(user["info"]["email"]))
            return WishlistStatus::PENDING;
        else if (fyps[fyp_id.parse_string(0, false, true)]["wishlistApproved"].contains(user["info"]["email"]))
            return WishlistStatus::APPROVED;
    }
    return WishlistStatus::UNKNOWN;
}

string get_pending_wishlist_amount(const string& fyp_id) {
    if (fyps[fyp_id]["wishlistPending"].size() > 99) {
        return "[99+]";
    } else {
        return "[" + to_string(fyps[fyp_id]["wishlistPending"].size()) + "]";
    }
}

vector<json> get_user_approved_fyps() {
    vector<json> approved_fyps;

    for (auto& [key, val] : fyps.as_dictionary()) {
        if (val["wishlistApproved"].contains(user["info"]["email"])) {
            val["id"] = key;
            approved_fyps.push_back(val);
        }
    }
    
    return approved_fyps;
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
    if (previous_page.size() > 1)
        previous_page.pop_back();
}