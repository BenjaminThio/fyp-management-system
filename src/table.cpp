#include <string>
#include <vector>
#include <sstream>
#include "json.h"
#include "utils.h"
using namespace std;

/*
static string strip_ansi(const string& s) {
    string out;
    out.reserve(s.size());
    size_t i = 0;
    while (i < s.size()) {
        if (s[i] == '\x1b') {
            ++i;
            if (i < s.size() && s[i] == '[') ++i;
            while (i < s.size() && s[i] != 'm') ++i;
            if (i < s.size() && s[i] == 'm') ++i;
        } else out.push_back(s[i++]);
    }
    return out;
}
*/

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