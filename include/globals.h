#ifndef GLOBALS_H
#define GLOBALS_H
#include "miniaudio.h"

enum class MainPageOption: int {
    FYP_TITLES,
    CONSOLE,
    ACCOUNT,
    EXIT
};

enum class Page: int {
    MAIN,
    FYPS,
    SIGN_UP,
    LOGIN,
    FYP,
    CONSOLE,
    EDIT,
    WISHLIST
};

extern int previousPage;
extern int page;
extern int selectedOption;
extern ma_engine sfxEngine;

#endif