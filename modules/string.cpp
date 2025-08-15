#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<string> split(string, char);
vector<char> string_to_chars(const string& s);

int main() {
    string text = "Benjamin Thio Zi Liang";
    vector<char> chars = string_to_chars(text);
    for (int i = 0; i < chars.size(); i++) {
        cout << chars[i] << endl;
    }
    return 0;
}

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

vector<char> string_to_chars(const string& s) {
    vector<char> chars;

    for (char c : s) {
        chars.push_back(c);
    }

    return chars;
}