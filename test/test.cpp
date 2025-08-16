#include <string>
#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

int main() {
    while (true) {
        if (_kbhit()) {
            int key = _getch();

            switch (key) {
                case 0:
                case 224: {
                    int special_key = 0;

                    cout << "Special Key: " << special_key << endl;
                    break;
                }
                default:
                    if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
                        cout << "Key: " << key << endl;
                    break;
            }
        }
    }
}
