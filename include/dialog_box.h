#pragma once
#include <windows.h>
#include <string>

LPSTR dialog_box();
namespace dialog {
    void warning_message(string message);
    void error_message(string message);
}
