#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <sstream>
using namespace std;

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
            unsigned char c = s[i];
            if (c == 0xE2 && i + 2 < s.size() &&
                (unsigned char)s[i+1] == 0x80 &&
                (unsigned char)s[i+2] == 0xA6) {
                i += 2;
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

string overlay(const string& front_buffer, const string& back_buffer, const size_t x_offset = 0) {
    // Front array buffer
    vector<string> f = split(front_buffer, '\n');
    // Back array buffer
    vector<string> b = split(back_buffer, '\n');
    string result;

    if (b.size() > f.size()) {
        for (size_t i = 0; i < b.size() - f.size(); i++) f.resize(b.size(), "");
    }

    for (size_t y = 0; y < b.size(); y++) {
        if (f[y].size() < x_offset + b[y].size()) f[y].resize(x_offset + b[y].size(), ' ');

        for (size_t x = 0; x < b[y].size(); x++) f[y][x_offset + x] = b[y][x];
    }

    for (size_t i = 0; i < f.size(); i++) {
        result += f[i];
        if (i + 1 < f.size()) result += '\n';
    };

    return result;
}

bool collapse = false;
enum class Option: int {
    SHOW_ALL,
    FIVE,
    TEN,
    TWENTY,
    FIFTY,
    ONE_HUNDRED
};
int selected_option = static_cast<int>(Option::SHOW_ALL);
vector<string> options = { "Show All", "5", "10", "20", "50", "100" };

int main() {
    string front_buffer = string(50, 'A') + "\n" + string(50, 'B') + "\n" + string(50, 'C') + "\n" + string(50, 'D') + "\n" + string(50, 'E') + "\n" + string(50, 'F') + "\n" + string(50, 'G');

    while (true) {
        if (_kbhit()) {
            int key = _getch();
            switch (key) {
                case 0:
                case 224:
                    break;
                case 13:
                    system("cls");
                    collapse = !collapse;
                    vector<vector<string>> table = { { options[selected_option] } }; // generate_table({ {string(5, ' ') + "A" + string(5, ' ')}, {string(5, ' ') + "B" + string(5, ' ')} });

                    if (collapse) {
                        for (const auto& s : options) table.push_back({s});
                    }

                    string back_buffer = generate_table(table);
                    cout << overlay(front_buffer, back_buffer, 20);
                    break;
            }
        }
    }

    return 0;
}