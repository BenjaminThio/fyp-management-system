#pragma once

#include <sstream>
#include <filesystem>
using namespace std;
namespace fs = filesystem;

namespace submit {
    void refresh_approved_fyps();
    vector<fs::path> get_files(fs::path path);
    fs::path get_file(fs::path path);
    void submit(fs::path source, const string& email);
    void push_frame(ostringstream& renderer);
    void keyboard_input_callback();
}