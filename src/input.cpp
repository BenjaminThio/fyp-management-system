#include <iostream>
#include <windows.h>
using namespace std;

void setInputCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void test() {
    string userInput;

    cout << "User Input: " << '"' << userInput << '"' << endl;
    cout << "Username:" << endl << endl << "Password:" << endl << endl << "Email:" << endl;
    setInputCursorPosition(0, 2);
    getline(cin, userInput);
}