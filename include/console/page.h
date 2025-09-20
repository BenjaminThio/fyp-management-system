#pragma once

#include <sstream>
#include "json.h"
using namespace std;

namespace console {
    enum class SelectOption: int {
        SHOW_ALL,
        FIVE,
        TEN,
        TWENTY,
        FIFTY,
        ONE_HUNDRED
    };
    enum class Option: int {
        PUBLIC,
        VIEW,
        EDIT,
        WISHLIST_APPROVAL,
        SUMMARY,
        DELETEE
    };
    enum class Direction: int {
        LEFT,
        RIGHT
    };
    void refresh_fyps_data(void);
    void push_frame(ostringstream& renderer);
    void keyboard_input_callback(void);
}