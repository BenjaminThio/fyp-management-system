#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <filesystem>
#include <vector>
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

namespace fs = filesystem;

static void create_default_user() {
    json j = json::dictionary{};
    string email = "benjaminthio@utaradmin.com";

    j[email] = json::dictionary{
        { "fyps", json::list{} },
        {
            "info", json::dictionary{
                { "username", "Benjamin Thio Zi Liang" },
                { "password", "@BenjaminThio123" },
                { "email", email }
            }
        }
    };
    if (is_student(email)) {
        j[email]["wishlist"] = json::list{};
    }
    save("../data/user.json", j);
}

static void ensure_json_files(const fs::path& folder, const vector<string>& files) {
    if (!fs::exists(folder)) {
        fs::create_directories(folder);
    }

    for (const auto& file : files) {
        fs::path file_path = folder / fs::path(file);

        if (!fs::exists(file_path)) {
            ofstream ofs(file_path);
            if (file == "fyp.json") {
                fyp::create_default_fyp_data();
            } else if (file == "session.json") {
                ofs << R"({"sessionId": null})";
            } else {
                create_default_user();
            }
        }
    }
}

int main() {
    vector<string> json_files = {
        "fyp.json",
        "session.json",
        "user.json"
    };
    ensure_json_files(DESTINATION, json_files);
    update_data();
    update_public_fyps();

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