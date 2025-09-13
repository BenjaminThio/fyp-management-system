#include <iostream>
#include <windows.h>
#include <keyboard.h>
#include <string>
#include <conio.h>
#include <renderer.h>
#include <array>
#include "text_formatter.h"
using namespace std;
using namespace ansi;

void set_input_cursor_position(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

string render_static_input_field(const string& input, const size_t length, const TextFormat& bg_color, const TextFormat& fg_color) {
    if (input.length() < length)
        return format(input + string(length - input.length(), ' '), bg_color, fg_color);
    else if (input.length() == length)
        return format(input, bg_color, fg_color);
    else
        return format(input.substr(0, length - 1), bg_color, fg_color) + "…"; // "...";
}

string render_input_field(const string& input, const int local_caret_pos, const int input_field_view_offset, const size_t length, const TextFormat& bg_color, const TextFormat& fg_color) {
    if (input.length() < length)
        return format(input + string(length - input.length(), ' '), bg_color, fg_color);
    else if (input.length() == length)
        return format(input, bg_color, fg_color);
    else
        return format(input.substr(input_field_view_offset, input_field_view_offset + local_caret_pos < length || input_field_view_offset + local_caret_pos > length ? length : input_field_view_offset + local_caret_pos), bg_color, fg_color);
}

array<int, 2> int_input_field(string& input, int& local_caret_pos, int& input_field_view_offset, const size_t max_length, const size_t length, size_t max_value) {
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
                                if (local_caret_pos - 1 >= 0)
                                    local_caret_pos--;
                                else if (input_field_view_offset - 1 >= 0)
                                    input_field_view_offset--;
                                break;
                            case (static_cast<int>(Key::RIGHT)):
                                if (local_caret_pos + 1 <= (input.length() < length ? input.length() : length))
                                    local_caret_pos++;
                                else if (local_caret_pos + input_field_view_offset + 1 <= input.length())
                                    input_field_view_offset++;
                                break;
                        }

                        render_page();
                        break;
                }
                break;
            }
            default:
                if (key >= 33 && key <= 126) {
                    string temp_input = input;

                    temp_input.insert(input_field_view_offset + local_caret_pos, 1, (char)key);
                    temp_input = to_string(stoi(temp_input));

                    if (max_length == 0 || max_length > 0 && temp_input.size() <= max_length) {
                        size_t prev_size = input.size();
                        input.insert(input_field_view_offset + local_caret_pos, 1, (char)key);

                        if (max_value > 0 && stoi(input) > max_value) {
                            input = to_string(max_value);
                        }

                        if (prev_size == to_string(stoi(input)).size()) {
                            input = to_string(stoi(input));
                        } else {
                            if (local_caret_pos + 1 <= length)
                                local_caret_pos++;
                            else if (local_caret_pos + input_field_view_offset + 1 <= input.length())
                                input_field_view_offset++;
                        }

                        render_page();
                    }
                } else {
                    switch (key) {
                        case static_cast<int>(Key::BACKSPACE):
                            if (input_field_view_offset - 1 >= 0 || local_caret_pos - 1 >= 0) {
                                input.erase(input_field_view_offset + local_caret_pos - 1, 1);

                                if (input == "")
                                    input = "0";
                                else {
                                    if (input_field_view_offset - 1 >= 0)
                                        input_field_view_offset--;
                                    else if (local_caret_pos - 1 >= 0)
                                        local_caret_pos--;
                                }

                                render_page();
                            }
                            break;
                    }
                }
                break;
        }
    }

    return { key, special_key };
}

array<int, 2> input_field(string& input, int& local_caret_pos, int& input_field_view_offset, bool allow_spaces, const size_t max_length, const size_t length) {
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
                                if (local_caret_pos - 1 >= 0)
                                    local_caret_pos--;
                                else if (input_field_view_offset - 1 >= 0)
                                    input_field_view_offset--;
                                break;
                            case (static_cast<int>(Key::RIGHT)):
                                if (local_caret_pos + 1 <= (input.length() < length ? input.length() : length))
                                    local_caret_pos++;
                                else if (local_caret_pos + input_field_view_offset + 1 <= input.length())
                                    input_field_view_offset++;
                                break;
                        }

                        render_page();
                        break;
                }
                break;
            }
            default:
                if (key >= (allow_spaces ? 32 : 33) && key <= 126) {
                    if (max_length == 0 || max_length > 0 && input.length() + 1 <= max_length) {
                        input.insert(input_field_view_offset + local_caret_pos, 1, (char)key);

                        if (local_caret_pos + 1 <= length)
                            local_caret_pos++;
                        else if (local_caret_pos + input_field_view_offset + 1 <= input.length())
                            input_field_view_offset++;

                        render_page();
                    }
                } else {
                    switch (key) {
                        case static_cast<int>(Key::BACKSPACE):
                            if (input_field_view_offset - 1 >= 0 || local_caret_pos - 1 >= 0) {
                                input.erase(input_field_view_offset + local_caret_pos - 1, 1);

                                if (input_field_view_offset - 1 >= 0)
                                    input_field_view_offset--;
                                else if (local_caret_pos - 1 >= 0)
                                    local_caret_pos--;

                                render_page();
                            }
                            break;
                    }
                }
                break;
        }
    }

    return { key, special_key };
}

array<int, 2> better_cin(string& input, int& input_cursor_pos, bool allow_spaces, size_t max_length /* , int x_offset, int y_offset */) {
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
                        // set_input_cursor_position(input_cursor_pos + x_offset, y_offset);
                        break;
                }
                break;
            }
            default:
                if (key >= (allow_spaces ? 32 : 33) && key <= 126) {
                    if (max_length == 0 || max_length > 0 && input.length() + 1 <= max_length) {
                        input.insert(input_cursor_pos, 1, (char)key);
                        input_cursor_pos += 1;

                        render_page();
                        // set_input_cursor_position(input_cursor_pos + x_offset, y_offset);
                    }
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