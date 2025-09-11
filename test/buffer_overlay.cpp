#include <iostream>
#include <string>
#include <vector>
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

int main() {
    string str_1 = string(50, 'A') + "\n" + string(50, 'B') + "\n" + string(50, 'C') + "\n" + string(50, 'D') + "\n" + string(50, 'E') + "\n" + string(50, 'F') + "\n" + string(50, 'G');
    string str_2 = "+" + string(11, '-') + "+\n|" + "     A     |\n+" + string(11, '-') + "+\n|" + "     B     |\n+" + string(11, '-') + "+";

    cout << overlay(str_1, str_2, 40);

    return 0;
}