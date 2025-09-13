#ifndef GLOBALS_H
#define GLOBALS_H
#include <string>
#include <vector>
#include "json.h"
using namespace std;

namespace globals {
    struct input_header {
        string field;
        int local_caret_pos;
        int input_field_view_offset;
        size_t min_length;
        size_t max_length;
        int length;
        bool error;
    };
    const string ADMIN_DOMAIN = "utaradmin.com";
    const string STUDENT_DOMAIN = "utarstudent.com";
}

struct coord_3d {
    int x = 0;
    int y = 0;
    int z = 0;
};

struct coord_2d {
    int x = 0;
    int y = 0;
};

enum class Page: int {
    HOME,
    FYP_LIST,
    SIGN_UP,
    LOGIN,
    FYP_VIEW,
    CONSOLE,
    CREATE_EDIT,
    WISHLIST,
    GRADING,
    NOT_FOUND
};

enum class Role: int {
    STUDENT,
    ADMIN
};

enum class WishlistStatus: int {
    PENDING,
    APPROVED,
    REJECTED,
    DISCARDED,
    UNKNOWN = -1
};

const int MAX_WISHLIST_AMOUNT = 5;
extern string session_id;
extern vector<int> previous_page;
extern int page;
extern json user;
extern json fyps;

bool is_authorized();
void update_data();
void clear_session();
json get_users();
json get_user();
void update_user();
void update_fyps();
WishlistStatus get_wishlist_status(const json& fyp_id);
bool is_student(const string& email);
bool is_admin(const string& email);
Role get_role();
json::list get_wishlist();
json::dictionary get_fyps();
void redirect(int p, const string& snd = "select", const bool incognito_mode = false);
void return_page(void);

#endif