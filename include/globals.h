#ifndef GLOBALS_H
#define GLOBALS_H
#include <string>
#include <vector>
#include "json.h"
using namespace std;

struct input_header {
    string field;
    int local_caret_pos;
    int input_field_view_offset;
    size_t min_length;
    size_t max_length;
    int length;
    bool error;
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

extern string session_id;
extern vector<int> previous_page;
extern int page;
extern json user;
extern json fyps;

void start();
json get_user();
json::list get_wishlist();
json::dictionary get_fyps();
void redirect(int p, const string& snd = "select", const bool incognito_mode = false);
void return_page(void);

#endif