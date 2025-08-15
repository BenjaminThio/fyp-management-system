#include <iostream>
#include <string>
#include "json.h"
#include "globals.h"
#include "renderer.h"
#include "keyboard.h"
#include "database.h"
#include "uuid_v4.h"
#include "audio.h"
#include "fyp/page.h"
#include "console/page.h"
using namespace std;

int main() {
    fyp::start();
    console::start();

    /*
    json j = json::dictionary{};

    for (size_t i = 0; i < 10; i++) {
        string uuid = UUIDv4::generate();

        j[uuid] = json::dictionary{
            { "Benjamin Thio Zi Liang", "I love u."} ,
            { "test", 1 }
        };
    }

    save("../data/test.json", j);

    //cout << j.parse_string() << endl;
    
    //save("../data/test.json", j);
    */

    if (ma_engine_init(NULL, &sfx_engine) != MA_SUCCESS) {
        cerr << "Failed to initialise SFX engine" << endl;
        getchar();
        return -1;
    }

    render_page();

    while (true) detect_keyboard_input();
}