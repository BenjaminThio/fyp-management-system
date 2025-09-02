#include <windows.h>
#include <iostream>
#include <string>
#include <conio.h>
#include <array>
#include <string_view>
#include <string>
#include <type_traits>
using namespace std;

struct TextFormat {
    string_view code;
};

namespace ansi {
    inline constexpr TextFormat DEFAULT { "\033[0m" };
    inline constexpr TextFormat FG_BLACK { "\033[30m" };
    inline constexpr TextFormat BG_BLACK { "\033[40m" };
    inline constexpr TextFormat FG_RED { "\033[31m" };
    inline constexpr TextFormat BG_RED { "\033[41m" };
    inline constexpr TextFormat FG_GREEN { "\033[32m" };
    inline constexpr TextFormat BG_GREEN { "\033[42m" };
    inline constexpr TextFormat FG_YELLOW { "\033[33m" };
    inline constexpr TextFormat BG_YELLOW { "\033[43m" };
    inline constexpr TextFormat FG_BLUE { "\033[34m" };
    inline constexpr TextFormat BG_BLUE { "\033[44m" };
    inline constexpr TextFormat FG_MAGENTA { "\033[35m" };
    inline constexpr TextFormat BG_MAGENTA { "\033[45m" };
    inline constexpr TextFormat FG_CYAN { "\033[36m" };
    inline constexpr TextFormat BG_CYAN { "\033[46m" };
    inline constexpr TextFormat FG_LIGHT_GRAY { "\033[37m" };
    inline constexpr TextFormat BG_LIGHT_GRAY { "\033[47m" };
    inline constexpr TextFormat FG_DARK_GRAY { "\033[90m" };
    inline constexpr TextFormat BG_DARK_GRAY { "\033[100m" };
    inline constexpr TextFormat FG_LIGHT_RED { "\033[91m" };
    inline constexpr TextFormat BG_LIGHT_RED { "\033[101m" };
    inline constexpr TextFormat FG_LIGHT_GREEN { "\033[92m" };
    inline constexpr TextFormat BG_LIGHT_GREEN { "\033[102m" };
    inline constexpr TextFormat FG_LIGHT_YELLOW { "\033[93m" };
    inline constexpr TextFormat BG_LIGHT_YELLOW { "\033[103m" };
    inline constexpr TextFormat FG_LIGHT_BLUE { "\033[94m" };
    inline constexpr TextFormat BG_LIGHT_BLUE { "\033[104m" };
    inline constexpr TextFormat FG_LIGHT_MAGENTA { "\033[95m" };
    inline constexpr TextFormat BG_LIGHT_MAGENTA { "\033[105m" };
    inline constexpr TextFormat FG_LIGHT_CYAN { "\033[96m" };
    inline constexpr TextFormat BG_LIGHT_CYAN { "\033[106m" };
    inline constexpr TextFormat FG_WHITE { "\033[97m" };
    inline constexpr TextFormat BG_WHITE { "\033[107m" };
    inline constexpr TextFormat BOLD { "\033[1m" };
    inline constexpr TextFormat UNDERLINE { "\033[4m" };
    inline constexpr TextFormat NO_UNDERLINE { "\033[24m" };
    inline constexpr TextFormat REVERSE_TEXT { "\033[7m" };
    inline constexpr TextFormat POSITIVE_TEXT { "\033[27m" };

    template <typename... TextFormats>
    string format(string text, TextFormats... text_formats) {
        static_assert((is_same_v<TextFormats, TextFormat> && ...), "The rest of the params should be in the type of `TextFormat`.");

        string result;

        ((result += text_formats.code), ...);
        result += text;
        result += DEFAULT.code;

        return result;
    }
}

enum class Key: int {
    UP = 72,
    LEFT = 75,
    DOWN = 80,
    RIGHT = 77,
    ENTER = 13,
    ESCAPE = 27,
    BACKSPACE = 8,
    TAB = 9
};
const int input_field_length = 30;

void set_input_cursor_position(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

string render_input_field(const string& text, const int local_caret_pos, const int input_field_view_offset, const size_t length) {
    if (text.length() < length)
        return ansi::format(text + string(length - text.length(), ' '), ansi::BG_WHITE, ansi::FG_BLACK);
    else if (text.length() == length)
        return ansi::format(text, ansi::BG_WHITE, ansi::FG_BLACK);
    else
        return ansi::format(text.substr(input_field_view_offset, input_field_view_offset + local_caret_pos < length || input_field_view_offset + local_caret_pos > length ? length : input_field_view_offset + local_caret_pos), ansi::BG_WHITE, ansi::FG_BLACK);
}

array<int, 2> input_field(string& input, int& local_caret_pos, int& input_field_view_offset, bool allow_spaces = true, const size_t max_length = 0, const size_t length = 0) {
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
                        system("cls");

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

                        cout << render_input_field(input, local_caret_pos, input_field_view_offset, length)
                            << endl << "Input: " << input
                            << endl << "Local Caret Pos: " << local_caret_pos
                            << endl << "Input Field View Offset: " << input_field_view_offset
                            << endl << "Total: " << input_field_view_offset + local_caret_pos
                            << endl << "Params: " << input_field_view_offset << ',' << (input_field_view_offset, input_field_view_offset + local_caret_pos < length || input_field_view_offset + local_caret_pos > length ? length : input_field_view_offset + local_caret_pos);
                        set_input_cursor_position(local_caret_pos, 0);
                        break;
                }
                break;
            }
            default:
                if (key >= (allow_spaces ? 32 : 33) && key <= 126) {
                    if (max_length == 0 || max_length > 0 && input.length() + 1 <= max_length) {
                        system("cls");
                        input.insert(input_field_view_offset + local_caret_pos, 1, (char)key);

                        if (local_caret_pos + 1 <= length)
                            local_caret_pos++;
                        else if (local_caret_pos + input_field_view_offset + 1 <= input.length())
                            input_field_view_offset++;

                        cout << render_input_field(input, local_caret_pos, input_field_view_offset, length)
                            << endl << "Input: " << input
                            << endl << "Local Caret Pos: " << local_caret_pos
                            << endl << "Input Field View Offset: " << input_field_view_offset
                            << endl << "Total: " << input_field_view_offset + local_caret_pos
                            << endl << "Params: " << input_field_view_offset << ',' << (input_field_view_offset, input_field_view_offset + local_caret_pos < length || input_field_view_offset + local_caret_pos > length ? length : input_field_view_offset + local_caret_pos);
                        set_input_cursor_position(local_caret_pos, 0);
                    }
                } else {
                    switch (key) {
                        case static_cast<int>(Key::BACKSPACE):
                            if (input_field_view_offset - 1 >= 0 || local_caret_pos - 1 >= 0) {
                                system("cls");
                                input.erase(input_field_view_offset + local_caret_pos - 1, 1);

                                if (input_field_view_offset - 1 >= 0)
                                    input_field_view_offset--;
                                else if (local_caret_pos - 1 >= 0)
                                    local_caret_pos--;

                                cout << render_input_field(input, local_caret_pos, input_field_view_offset, length)
                                << endl << "Input: " << input
                                << endl << "Local Caret Pos: " << local_caret_pos
                                << endl << "Input Field View Offset: " << input_field_view_offset
                                << endl << "Total: " << input_field_view_offset + local_caret_pos
                                << endl << "Params: " << input_field_view_offset << ',' << (input_field_view_offset, input_field_view_offset + local_caret_pos < length || input_field_view_offset + local_caret_pos > length ? length : input_field_view_offset + local_caret_pos);
                                set_input_cursor_position(local_caret_pos, 0);
                            }
                            break;
                    }
                }
                break;
        }
    }

    return { key, special_key };
}

int main() {
    string input;
    int local_caret_pos = 0;
    int input_field_view_offset = 0;

    cout << render_input_field(input, local_caret_pos, input_field_view_offset, input_field_length)
    << endl << "Input: " << input
    << endl << "Local Caret Pos: " << local_caret_pos
    << endl << "Input Field View Offset: " << input_field_view_offset
    << endl << "Total: " << input_field_view_offset + local_caret_pos
    << endl << "Params: " << input_field_view_offset << ',' << (input_field_view_offset, input_field_view_offset + local_caret_pos < input_field_length || input_field_view_offset + local_caret_pos > input_field_length ? input_field_length : input_field_view_offset + local_caret_pos);
    set_input_cursor_position(local_caret_pos, 0);
    while (true) {
        array<int, 2> keyboard_input = input_field(input, local_caret_pos, input_field_view_offset, true, 0, input_field_length);
        // int key = keyboard_input[0];
        // int special_key = keyboard_input[1];
    }
    return 0;
}