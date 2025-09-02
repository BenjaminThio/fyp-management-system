#include <iostream>
#include <windows.h>
using namespace std;

namespace terminal {
    void hide_cursor() {
        HANDLE h_out = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursor_info;
        GetConsoleCursorInfo(h_out, &cursor_info);
        cursor_info.bVisible = FALSE;
        SetConsoleCursorInfo(h_out, &cursor_info);
    }

    void show_cursor() {
        HANDLE h_out = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursor_info;
        GetConsoleCursorInfo(h_out, &cursor_info);
        cursor_info.bVisible = TRUE;
        SetConsoleCursorInfo(h_out, &cursor_info);
    }

    void maximize_window() {
        HWND console = GetConsoleWindow();
        ShowWindow(console, SW_MAXIMIZE);
    }
}

int main() {
    terminal::maximize_window();
    terminal::hide_cursor();
    cout << "Benjamin Thio Zi Liang" << endl;
    getchar();
    return 0;
}