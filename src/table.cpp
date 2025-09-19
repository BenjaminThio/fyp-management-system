#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <windows.h>
#include <cwchar>
#include <cstdint> // For uint32_t
#include "json.h"
#include "utils.h"
using namespace std;

// Minimal wcwidth fallback (works on Windows + emoji fix)
static int wcwidth_fallback(uint32_t ucs) {  // Changed to uint32_t for full code points
    if (ucs == 0) return 0;
    if (ucs < 32 || (ucs >= 0x7f && ucs < 0xa0)) return -1; // control chars

    // ---- Manual overrides (ambiguous emojis that Windows console shows as width=1) ----
    switch (ucs) {
        case 0x2B50: return 1; // ⭐ Star
        case 0x274C: return 2; // ❌ Cross Mark
        case 0x2705: return 1; // ✅ Check Mark Button
        case 0x26A0: return 1; // ⚠ Warning
        case 0x2728: return 1; // ✨ Sparkles
        case 0x2B55: return 1; // ⭕ Heavy Circle
        case 0x1F441: return 1; // 👁 Eye  <--- Set to 1 to match what works in your second code
        case 0x30C4: return 2;
    }

    // ---- East Asian Wide ranges (always width=2) ----
    if ((ucs >= 0x1100 && ucs <= 0x115f) || // Hangul Jamo
        (ucs >= 0x231A && ucs <= 0x231B) || // ⌚ ⌛
        (ucs == 0x2329 || ucs == 0x232A) || // angle brackets
        (ucs >= 0x23E9 && ucs <= 0x23EC) || // media controls
        (ucs == 0x23F0 || ucs == 0x23F3) || // alarm clock, hourglass
        (ucs >= 0x25FD && ucs <= 0x25FE) || // ◽ ◾
        (ucs >= 0x2614 && ucs <= 0x2615) || // ☔ ☕
        (ucs >= 0x2648 && ucs <= 0x2653) || // zodiac signs
        (ucs == 0x267F) ||                  // ♿
        (ucs >= 0x2693 && ucs <= 0x26A1) || // ⚓ ... ⚡
        (ucs >= 0x26AA && ucs <= 0x26AB) || // ⚪ ⚫
        (ucs >= 0x26BD && ucs <= 0x26BE) || // ⚽ ⚾
        (ucs >= 0x26C4 && ucs <= 0x26C5) || // ⛄ ☁
        (ucs == 0x26CE) ||                  // ⛎
        (ucs >= 0x26D4 && ucs <= 0x26F5) || // ⛔ ... ⛵
        (ucs >= 0x26FA && ucs <= 0x26FD) || // ⛺ ⛽
        (ucs >= 0x270A && ucs <= 0x270B) || // ✊ ✋
        (ucs == 0x2728) ||                  // ✨
        (ucs >= 0x2744 && ucs <= 0x2747) || // ❄ ❇
        (ucs == 0x274C || ucs == 0x274E) || // ❌ ❎
        (ucs >= 0x2753 && ucs <= 0x2755) || // ❓ ❔ ❕ ❗
        (ucs == 0x2757) ||                  // ❗
        (ucs >= 0x2795 && ucs <= 0x2797) || // ➕ ➖ ➗
        (ucs == 0x27B0 || ucs == 0x27BF) || // ➰ ➿
        (ucs >= 0x2B1B && ucs <= 0x2B1C) || // ⬛ ⬜
        (ucs == 0x2B50) ||                  // ⭐
        (ucs == 0x2B55) ||                  // ⭕
        (ucs >= 0x1F300 && ucs <= 0x1FAFF)) // full emoji range
    {
        return 2;
    }

    return 1; // default single-width
}

// ---------------- UTF-8 & width helpers ----------------

// UTF-8 -> wstring using Windows API
static std::wstring utf8_to_wstring(const std::string& str) {
    if (str.empty()) return L"";
    int size_needed = MultiByteToWideChar(
        CP_UTF8, 0, str.c_str(), (int)str.size(), nullptr, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(
        CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);
    return wstr;
}

static int utf8_width(const std::string& s) {
    std::wstring ws = utf8_to_wstring(s);
    int width = 0;
    for (size_t i = 0; i < ws.size(); i++) {
        wchar_t wc = ws[i];
        uint32_t cp = static_cast<uint32_t>(wc);  // Default to single wchar_t
        if (wc >= 0xD800 && wc <= 0xDBFF && i + 1 < ws.size()) {
            wchar_t next_wc = ws[i + 1];
            if (next_wc >= 0xDC00 && next_wc <= 0xDFFF) {
                // Decode full code point
                cp = ((static_cast<uint32_t>(wc) - 0xD800) << 10) +
                     (static_cast<uint32_t>(next_wc) - 0xDC00) + 0x10000;
                i++; // Skip the low surrogate
            }
        }
        int w = wcwidth_fallback(cp);
        if (w < 0) w = 0; // ignore control chars
        width += w;
    }
    return width;
}

// ---------------- Helpers ----------------

static pair<string, size_t> next_utf8_char(const string& input, size_t raw_pos) {
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
        throw runtime_error("Invalid UTF-8 sequence");
    }

    return {input.substr(raw_pos, len), raw_pos + len};
}

static string strip_ansi(const string& s) {
    string out;
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

static size_t visible_length(const string& s) {
    size_t length = 0;
    size_t i = 0;

    while (i < s.size()) {
        if (s[i] == '\x1b') {
            // Skip ANSI escape codes
            i++;
            if (i < s.size() && s[i] == '[') {
                i++;
                while (i < s.size() && (s[i] < 0x40 || s[i] > 0x7E)) i++;
                if (i < s.size()) i++;
            }
        } else {
            // Extract next UTF-8 char
            auto [ch, next_pos] = next_utf8_char(s, i);
            // Measure its actual display width
            length += utf8_width(ch);
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
            renderer << '+';
        }
        renderer << endl
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
        renderer << '+';
    }
    renderer << endl;

    return renderer.str();
}