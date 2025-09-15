#include <windows.h>
#include <shellapi.h>
#include <sstream>
#include <conio.h>
#include <filesystem>
#include <stdexcept>
#include "keyboard.h"
#include "globals.h"
#include "renderer.h"
#include "audio.h"
#include "table.h"
#include "dialog_box.h"
#include "text_formatter.h"
using namespace std;
using namespace ansi;

namespace fs = filesystem;

namespace submit {
    fs::path get_exe_dir() {
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);

        return fs::path(buffer).parent_path();
    }

    static const fs::path DESTINATION = get_exe_dir().parent_path() / "data";
    static coord_2d selected_coord = { 1, 0 };
    static vector<json> approved_fyps;

    void refresh_approved_fyps() {
        approved_fyps = get_user_approved_fyps();
    }

    vector<fs::path> get_files(fs::path path) {
        vector<fs::path> paths;

        for (const auto& entry : fs::directory_iterator(path)) {
            paths.push_back(entry.path());
        }

        return paths;
    }

    fs::path get_file(fs::path path) {
        vector<fs::path> paths;

        for (const auto& entry : fs::directory_iterator(path)) {
            return entry.path();
        }
        return fs::path();
    }

    void open_file(const fs::path& filePath) {
        if (!fs::exists(filePath)) {
            throw runtime_error("File does not exist: " + filePath.string());
        }

        HINSTANCE result = ShellExecute(
            NULL,
            "open",
            filePath.string().c_str(),
            NULL,
            NULL,
            SW_SHOWNORMAL
        );

        if ((intptr_t)result <= 32) {
            throw runtime_error("Failed to open file: " + filePath.string());
        }
    }

    void submit(fs::path source, const string& email) {
        fs::path destination = DESTINATION / fs::path(email) / fs::path(approved_fyps[selected_coord.y]["id"].as_string());

        if (!fs::exists(destination)) {
            fs::create_directories(destination);
        }

        if (!fs::is_empty(destination)) {
            for (const auto& entry : fs::directory_iterator(destination)) {
                fs::remove(entry);
            }
        }

        fs::copy_file(source, destination / source.filename(), fs::copy_options::overwrite_existing);
    }
    
    void push_frame(ostringstream& renderer) {
        vector<vector<string>> table = { { " __   _     ___   _      _   __   __   _   ___   _     \n( (` | | | | |_) | |\\/| | | ( (` ( (` | | / / \\ | |\\ | \n_)_) \\_\\_/ |_|_) |_|  | |_| _)_) _)_) |_| \\_\\_/ |_| \\| " } };

        for (size_t i = 0; i < approved_fyps.size(); i++) {
            fs::path destination = DESTINATION / fs::path(user["info"]["email"].as_string()) / fs::path(approved_fyps[i]["id"].as_string());

            table.push_back({ 
                "Title: " + approved_fyps[i]["info"]["name"].as_string() +
                "\nID: " + approved_fyps[i]["id"].as_string() +
                "\nSubmitted File: " + (
                    fs::exists(destination) && !fs::is_empty(destination)
                    ?
                    format(get_file(destination).filename().string(),
                    (selected_coord.x == 0 && selected_coord.y == i ? FG_BLACK : FG_WHITE),
                    (selected_coord.x == 0 && selected_coord.y == i ? BG_GREEN : BG_BLACK),
                    UNDERLINE) + (selected_coord.x == 0 && selected_coord.y == i ? " 👁" : "")
                    :
                    "None"
                ),
                "\n" + (
                    fs::exists(destination) && !fs::is_empty(destination)
                    ?
                    (selected_coord.x == 1 && selected_coord.y == i ? format("Edit", FG_BLACK, BG_YELLOW) : "Edit")
                    :
                    (selected_coord.x == 1 && selected_coord.y == i ? format("Submit", FG_BLACK, BG_GREEN) : "Submit")
                )
            });
        }
        renderer << generate_table(table) << endl;
    }

    void keyboard_input_callback() {
        if (_kbhit()) {
            int key = _getch();

            switch (key) {
                case 0:
                case 224: {
                    int special_key = _getch();

                    switch (special_key) {
                        case static_cast<int>(Key::UP):
                            if (selected_coord.x == 0) return;

                            if (selected_coord.y - 1 >= 0)
                                selected_coord.y--;
                            else
                                selected_coord.y = approved_fyps.size() - 1;
                            
                            render_page();
                            break;
                        case static_cast<int>(Key::DOWN):
                            if (selected_coord.x == 0) return;

                            if (selected_coord.y + 1 < approved_fyps.size())
                                selected_coord.y++;
                            else
                                selected_coord.y = 0;
                            
                            render_page();
                            break;
                        case static_cast<int>(Key::LEFT):
                        case static_cast<int>(Key::RIGHT): {
                            fs::path destination = DESTINATION / fs::path(user["info"]["email"].as_string()) / fs::path(approved_fyps[selected_coord.y]["id"].as_string());

                            if (!fs::exists(destination) || fs::is_empty(destination)) return;

                            selected_coord.x = static_cast<int>(!static_cast<bool>(selected_coord.x));
                            render_page();
                            break;
                        }
                    }
                    break;
                }
                case static_cast<int>(Key::ENTER): {
                    if (selected_coord.x == 0) {
                        fs::path destination = DESTINATION / fs::path(user["info"]["email"].as_string()) / fs::path(approved_fyps[selected_coord.y]["id"].as_string());

                        open_file(destination / get_file(destination));
                        return;
                    }

                    fs::path source = dialog_box();

                    if (!source.empty()) {
                        submit(source, user["info"]["email"].as_string());
                        render_page();
                    }
                    break;
                }
                case static_cast<int>(Key::ESCAPE): {
                    return_page();
                    break;
                }
            }
        }
    }
}