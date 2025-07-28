#include <iostream>
#include <conio.h>
using namespace std;

int main() {
    while (true) {
        if (_kbhit()) {
            int key = _getch();

            if (key == 224) {
                int special_key = _getch();

                cout << "Special Key:" << special_key << endl;
            }
            else {
                cout << "Key: " << key << endl;
            }
        }
    }
    return 0;
}