#pragma once
#include <windows.h>
#include <string>
#include <filesystem>
using namespace std;

namespace fs = filesystem;

fs::path dialog_box();
namespace dialog {
    void warning_message(string message);
    void error_message(string message);
}
