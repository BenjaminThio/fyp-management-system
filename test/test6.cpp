#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <string_view>
#include <type_traits>
using namespace std;

struct TextFormat {
    string_view code;
};

vector<string> split(string text, char delimiter) {
    vector<string> segments;
    size_t pos = 0;

    while ((pos = text.find(delimiter)) != string::npos) {
        segments.push_back(text.substr(0, pos));
        text.erase(0, pos + 1);
    }

    if (text.length() > 0) {
        segments.push_back(text);
    }

    return segments;
}

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

static string strip_ansi(const string& s) {
    string out;
    out.reserve(s.size());
    size_t i = 0;
    while (i < s.size()) {
        if (s[i] == '\x1b') {
            ++i;
            if (i < s.size() && s[i] == '[') {
                ++i;
                while (i < s.size() && (s[i] < 0x40 || s[i] > 0x7E))
                    ++i;
                if (i < s.size()) ++i;
            }
        } else {
            // skip Unicode ellipsis (U+2026) if present
            unsigned char c = s[i];
            if (c == 0xE2 && i + 2 < s.size() &&
                (unsigned char)s[i+1] == 0x80 &&
                (unsigned char)s[i+2] == 0xA6) {
                i += 2; // skip '…'
            } else {
                out.push_back(s[i++]);
            }
        }
    }
    return out;
}

static size_t visible_length(const string& s) {
    return strip_ansi(s).length();
}

string generate_table(vector<vector<string>> table) {
    ostringstream renderer;
    size_t longest_row = 0;
    vector<size_t> row_heights;
    
    for (const auto& row : table) {
        longest_row = max(longest_row, row.size());
        row_heights.push_back(1);
    }

    vector<size_t> column_widths(longest_row, 0);

    for (size_t r = 0; r < table.size(); r++) {
        const auto& row = table[r];

        for (size_t c = 0; c < row.size(); c++) {
            vector<string> split_newline = split(row[c], '\n');
            
            if (split_newline.size() == 1)
                column_widths[c] = max(column_widths[c], visible_length(row[c]));
            else if (split_newline.size() > 1) {
                for (const string& text : split_newline)
                    column_widths[c] = max(column_widths[c], visible_length(text));
                row_heights[r] = max(row_heights[r], split_newline.size());
            }
        }
    }

    for (size_t r = 0; r < table.size(); r++) {
        const auto& row = table[r];

        renderer << '+';
        for (size_t i = 0; i < column_widths.size(); i++) {
            renderer << string(column_widths[i], '-');
            if (i < column_widths.size() - 1) renderer << '-';
        }
        renderer << '+' << endl
        << '|';
        
        if (row_heights[r] > 1) {
            vector<vector<string>> split_newlines;

            for (size_t c = 0; c < longest_row; c++) {
                if (c < row.size())
                    split_newlines.push_back(split(row[c], '\n'));
                else
                    split_newlines.push_back({ "" });
            }
            for (size_t y = 0; y < row_heights[r]; y++) {
                for (size_t x = 0; x < split_newlines.size(); x++) {
                    if (y > split_newlines[x].size() - 1) {
                        renderer << string(column_widths[x], ' ') << '|';
                    } else {
                        renderer << split_newlines[x][y];
                        if (column_widths[x] > visible_length(split_newlines[x][y]))
                            renderer << string(column_widths[x] - visible_length(split_newlines[x][y]), ' ');
                        renderer << '|';
                    }
                }
                if (y < row_heights[r] - 1) renderer << endl << '|';
            }
        } else {
            for (size_t c = 0; c < longest_row; c++) {
                if (c < row.size()) {
                    renderer << row[c];
                    if (column_widths[c] > visible_length(row[c])) renderer << string(column_widths[c] - visible_length(row[c]), ' ');
                    renderer << '|';
                } else {
                    renderer << string(column_widths[c], ' ') << '|';
                }
            }
        }
        renderer << endl;
    }

    renderer << '+';
    for (size_t i = 0; i < column_widths.size(); i++) {
        renderer << string(column_widths[i], '-');
        if (i < column_widths.size() - 1) renderer << '-';
    }
    renderer << '+' << endl;

    return renderer.str();
}

int main() {
    cout << ansi::format(string(3, ' '), ansi::BG_BLUE) << endl << ansi::format(string(3, ' '), ansi::BG_BLUE);
    return 0;
}