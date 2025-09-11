#pragma once
#include <windows.h>

LPSTR dialog_box();
namespace dialog {
    void warning_message(const char* message);
    void error_message(const char* message);
}
