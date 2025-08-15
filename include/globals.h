#ifndef GLOBALS_H
#define GLOBALS_H

enum class Page: int {
    HOME,
    FYP_LIST,
    SIGN_UP,
    LOGIN,
    FYP_VIEW,
    CONSOLE,
    EDIT,
    WISHLIST,
    NOT_FOUND
};

extern int previous_page;
extern int page;

#endif