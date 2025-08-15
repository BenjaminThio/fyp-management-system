#pragma once

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
