#include <vector>
#include <string>
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

vector<string> split_2(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(str);
    while (getline(tokenStream, token, delimiter)) {
        if (!token.empty())
            tokens.push_back(token);
    }
    return tokens;
}