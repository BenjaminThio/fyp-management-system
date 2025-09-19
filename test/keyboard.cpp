#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
using namespace std;
/*
bool test = false;

void test1() {
    cout << test << endl;
    this_thread::sleep_for(chrono::milliseconds(3000));
    cout << test << endl;
}
*/
int main() {
    while (true) {
        if (_kbhit()) {
            int key = _getch();

            if (key == 224 || key == 0) {
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