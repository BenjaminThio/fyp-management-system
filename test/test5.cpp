#include <iostream>
#include <windows.h>

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::cout << u8"Hello: 你好, мир, 😀 🌍 🏳️‍🌈" << std::endl;
    getchar();
}