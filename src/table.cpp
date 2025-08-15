#include <string>
#include <vector>
#include <sstream>
using namespace std;

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

static int visible_length(const string& s) {
    return static_cast<int>(strip_ansi(s).length());
}

string generate_table(vector<vector<string>> table) {
    ostringstream renderer;
    size_t longest_row = 0;

    for (const auto& row : table)
        longest_row = max(longest_row, row.size());

    vector<int> column_widths(longest_row, 0);

    for (const auto& row : table) {
        for (size_t i = 0; i < row.size(); i++)
            column_widths[i] = max(column_widths[i], visible_length(row[i]));
    }

    for (const auto& row : table) {
        renderer << '+';
        for (size_t i = 0; i < column_widths.size(); i++) {
            renderer << string(column_widths[i], '-');
            if (i < column_widths.size() - 1)
                renderer << '-';
        }

        renderer << '+' << endl << '|';
        
        for (size_t i = 0; i < column_widths.size(); i++) {
            int pad;

            if (i < row.size()) {
                pad = column_widths[i] - visible_length(row[i]);
                renderer << row[i];
            }
            else pad = column_widths[i];
            
            if (pad > 0)
                renderer << string(pad, ' ');
            renderer << '|';
        }
        renderer << endl;
    }
    renderer << '+';
    for (size_t i = 0; i < column_widths.size(); i++) {
        renderer << string(column_widths[i], '-');
        if (i < column_widths.size() - 1)
            renderer << '-';
    }
    renderer << '+';
    
    return renderer.str();
}