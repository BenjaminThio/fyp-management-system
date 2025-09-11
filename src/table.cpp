#include <string>
#include <vector>
#include <sstream>
#include "json.h"
#include "utils.h"
using namespace std;

static std::pair<std::string, size_t> next_utf8_char(const std::string& input, size_t raw_pos) {
    unsigned char c = input[raw_pos];
    size_t len = 1;

    if ((c & 0x80) == 0x00) {
        len = 1; // ASCII
    } else if ((c & 0xE0) == 0xC0) {
        len = 2;
    } else if ((c & 0xF0) == 0xE0) {
        len = 3;
    } else if ((c & 0xF8) == 0xF0) {
        len = 4;
    } else {
        throw std::runtime_error("Invalid UTF-8 sequence");
    }

    return {input.substr(raw_pos, len), raw_pos + len};
}

static std::string strip_ansi(const std::string& s) {
    std::string out;
    size_t i = 0;

    while (i < s.size()) {
        if (s[i] == '\x1b') {
            i++;
            if (i < s.size() && s[i] == '[') {
                i++;
                while (i < s.size() && (s[i] < 0x40 || s[i] > 0x7E)) i++;
                if (i < s.size()) i++;
            }
        } else {
            auto [ch, next_pos] = next_utf8_char(s, i);
            out += ch;
            i = next_pos;
        }
    }
    return out;
}

static size_t visible_length(const std::string& s) {
    size_t length = 0;
    size_t i = 0;

    while (i < s.size()) {
        if (s[i] == '\x1b') {
            // Skip ANSI escape sequence
            i++;
            if (i < s.size() && s[i] == '[') {
                i++;
                while (i < s.size() && (s[i] < 0x40 || s[i] > 0x7E)) i++;
                if (i < s.size()) i++; // consume final command char
            }
        } else {
            // Count one Unicode character
            auto [ch, next_pos] = next_utf8_char(s, i);
            length++;
            i = next_pos;
        }
    }

    return length;
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