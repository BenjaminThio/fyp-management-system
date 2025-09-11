#include <string>
#include <vector>
#include <regex>
using namespace std;

struct Token {
    bool is_ansi;
    string value;
};

// Pre-tokenized line with mappings
struct Line {
    vector<Token> tokens;
    vector<size_t> visual_to_raw;
    size_t visible_length = 0;
};

// UTF-8 char split3ter
pair<string, size_t> next_utf8_char(const string& input, size_t raw_pos) {
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

// Tokenizer with ANSI handling
Line tokenize_line(const string& input) {
    Line line;
    static const regex ansi_regex("\x1B\\[[0-9;]*[A-Za-z]");

    sregex_iterator it(input.begin(), input.end(), ansi_regex);
    sregex_iterator end;

    size_t raw_pos = 0;

    while (raw_pos < input.size()) {
        bool is_ansi = false;

        for (auto match = it; match != end; ++match) {
            if (match->position() == raw_pos) {
                line.tokens.push_back({true, match->str()});
                raw_pos += match->length();
                is_ansi = true;
                break;
            }
        }

        if (!is_ansi) {
            auto [ch, next_pos] = next_utf8_char(input, raw_pos);
            line.tokens.push_back({false, ch});
            line.visual_to_raw.push_back(raw_pos);
            line.visible_length++;
            raw_pos = next_pos;
        }
    }

    return line;
}

size_t get_raw_index(const Line& line, size_t visual_index) {
    if (visual_index >= line.visual_to_raw.size())
        throw out_of_range("Visual index out of range");
    return line.visual_to_raw[visual_index];
}

vector<string> split3(const string& s, char delim) {
    vector<string> elems;
    string item;
    istringstream iss(s);
    while (getline(iss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

// Overlay function that preserves ANSI formatting
string overlay(const string& front_buffer, const string& back_buffer, size_t x_offset) {
    vector<string> f_lines = split3(front_buffer, '\n');
    vector<string> b_lines = split3(back_buffer, '\n');
    string result;

    if (b_lines.size() > f_lines.size())
        f_lines.resize(b_lines.size(), "");

    for (size_t y = 0; y < b_lines.size(); y++) {
        Line f_line = tokenize_line(f_lines[y]);
        Line b_line = tokenize_line(b_lines[y]);

        if (f_line.visible_length < x_offset + b_line.visible_length) {
            f_lines[y].resize(x_offset + b_line.visible_length, ' ');
            f_line = tokenize_line(f_lines[y]);
        }

        size_t visible_idx = 0;
        for (size_t t = 0; t < b_line.tokens.size(); t++) {
            const Token& tok = b_line.tokens[t];

            if (tok.is_ansi) {
                // Insert ANSI codes directly into the front buffer
                size_t f_raw = get_raw_index(f_line, x_offset + visible_idx);
                f_lines[y].insert(f_raw, tok.value);
                // Re-tokenize because string changed
                f_line = tokenize_line(f_lines[y]);
                continue;
            }

            // Overlay visible character
            size_t f_raw = get_raw_index(f_line, x_offset + visible_idx);
            f_lines[y].replace(f_raw, tok.value.size(), tok.value);

            visible_idx++;
        }

        // Reset formatting at end of line
        f_lines[y] += "\x1b[0m";
    }

    for (size_t i = 0; i < f_lines.size(); i++) {
        result += f_lines[i];
        if (i + 1 < f_lines.size()) result += '\n';
    }

    return result;
}

/*
// Buffer overlay
string overlay(const string& front_buffer, const string& back_buffer, const size_t x_offset) {
    // Front array buffer
    vector<string> f = split3(front_buffer, '\n');
    // Back array buffer
    vector<string> b = split3(back_buffer, '\n');
    string result;

    if (b.size() > f.size()) {
        for (size_t i = 0; i < b.size() - f.size(); i++) f.resize(b.size(), "");
    }

    for (size_t y = 0; y < b.size(); y++) {
        if (f[y].size() < x_offset + b[y].size()) f[y].resize(x_offset + b[y].size(), ' ');

        for (size_t x = 0; x < visible_length(b[y]); x++) f[y][get_raw_index(f[y], x_offset + x)] = b[y][get_raw_index(b[y], x)];
    }


    for (size_t i = 0; i < f.size(); i++) {
        result += f[i];
        if (i + 1 < f.size()) result += '\n';
    };

    return result;
}
*/