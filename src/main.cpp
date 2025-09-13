#include <iostream>
#include <string>
#include <thread>
#include "json.h"
#include "globals.h"
#include "renderer.h"
#include "keyboard.h"
#include "database.h"
#include "uuid_v4.h"
#include "audio.h"
#include "fyp/page.h"
#include "console/page.h"
#include "grading/page.h"
#include "terminal.h"
using namespace std;

/*
int main() {
    fyp::create_default_fyp_data();
    return 0;
}
*/

int main() {
    update_data();
    if (is_authorized()) {
        console::refresh_fyps_data();
        grading::start();
    }

    // terminal::maximize_window();
    terminal::hide_cursor();

    if (ma_engine_init(NULL, &sfx_engine) != MA_SUCCESS) {
        cerr << "Failed to initialise SFX engine" << endl;
        getchar();
        return -1;
    }

    render_page();
    
    while (true) detect_keyboard_input();

    return 0;
}