#include <iostream>
#include <string>
#include <conio.h>
#include <keyboard.h>
#include <windows.h>
using namespace std;

void set_input_cursor_position(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

string better_cin(bool allow_spaces = true, int x_offset = 0, int y_offset = 0) {
    string input;
    int input_cursor_pos = 0;

    while (true) {
        if (_kbhit()) {
            int key = _getch();

            switch (key) {
                case 0:
                case 224: {
                    int special_key = _getch();

                    switch (special_key) {
                        case (static_cast<int>(Key::LEFT)):
                        case (static_cast<int>(Key::RIGHT)):
                            switch (special_key) {
                                case (static_cast<int>(Key::LEFT)):
                                    if (input_cursor_pos - 1 >= 0)
                                        input_cursor_pos -= 1;
                                    break;
                                case (static_cast<int>(Key::RIGHT)):
                                    if (input_cursor_pos + 1 <= input.length())
                                        input_cursor_pos += 1;
                                    break;
                            }

                            set_input_cursor_position(input_cursor_pos, 0);
                            break;
                    }
                    break;
                }
                default:
                    if (key >= 32 && key <= 126) {
                        input.insert(input_cursor_pos, 1, (char)key);
                        input_cursor_pos += 1;

                        system("cls");
                        cout << input;
                        set_input_cursor_position(input_cursor_pos, 0);
                    } else {
                        switch (key) {
                            case static_cast<int>(Key::BACKSPACE):
                                if (input_cursor_pos - 1 >= 0) {
                                    input.erase(input_cursor_pos - 1, 1);
                                    input_cursor_pos -= 1;

                                    system("cls");
                                    cout << input;
                                    set_input_cursor_position(input_cursor_pos, 0);
                                }
                                break;
                            case static_cast<int>(Key::ENTER): return input;
                        }
                    }
                    break;
            }
        }
    }
}

int main() {
    cout << better_cin() << endl;
    getchar();
    return 0;
}