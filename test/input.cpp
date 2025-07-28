#include <iostream>
#include <conio.h>
#include <regex>
using namespace std;

int main() {
    while (true) {
        if (_kbhit()) {
            int key = _getch();
            
            switch (key) {
                case 224: {
                    int specialKey = _getch();
                    
                    cout << "Special Key: " << specialKey << endl;
                    break;
                }
                default: {
                    regex reg()

                    cout << "Key: " << key << endl;
                    break;
                }
            }
        }
    }
    
    return 0;
}