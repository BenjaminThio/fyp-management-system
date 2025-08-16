#include <iostream>
#include <windows.h>
#include <keyboard.h>
#include <string>
#include <conio.h>
#include <renderer.h>
#include <array>
using namespace std;

void set_input_cursor_position(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

array<int, 2> better_cin(string& input, int& input_cursor_pos, bool allow_spaces = true /* , int x_offset = 0, int y_offset = 0 */) {
    int key = -1;
    int special_key = -1;

    if (_kbhit()) {
        key = _getch();

        switch (key) {
            case 0:
            case 224: {
                special_key = _getch();

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

                        render_page();
                        //set_input_cursor_position(input_cursor_pos + x_offset, y_offset);
                        break;
                }
                break;
            }
            default:
                if (key >= (allow_spaces ? 32 : 33) && key <= 126) {
                    input.insert(input_cursor_pos, 1, (char)key);
                    input_cursor_pos += 1;

                    render_page();
                    // set_input_cursor_position(input_cursor_pos + x_offset, y_offset);
                } else {
                    switch (key) {
                        case static_cast<int>(Key::BACKSPACE):
                            if (input_cursor_pos - 1 >= 0) {
                                input.erase(input_cursor_pos - 1, 1);
                                input_cursor_pos -= 1;

                                render_page();
                                // set_input_cursor_position(input_cursor_pos + x_offset, y_offset);
                            }
                            break;
                    }
                }
                break;
        }
    }

    return { key, special_key };
}

void test() {
    string userInput;

    cout << "User Input: " << '"' << userInput << '"' << endl;
    cout << "Username:" << endl << endl << "Password:" << endl << endl << "Email:" << endl;
    set_input_cursor_position(0, 2);
    getline(cin, userInput);
}