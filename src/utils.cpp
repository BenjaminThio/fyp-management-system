#include <vector>
#include <string>
#include <sstream>
#include "json.h"
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

template <typename T>
vector<vector<T>> group(vector<T> elements, size_t step) {
    vector<vector<T>> result = { {} };
    int i = 0;

    for (const auto& element : elements) {
        if (i == step) {
            result.push_back({});
            i = 0;
        }
        result[result.size() - 1].push_back(element);
        i++;
    }

    return result;
}

vector<json::dictionary> group_dict(json dict, size_t step) {
    vector<json::dictionary> result = { json::dictionary{} };
    int i = 0;

    for (const auto& [key, value] : dict.as_dictionary()) {
        if (i == step) {
            result.push_back(json::dictionary{});
            i = 0;
        }
        result[result.size() - 1][key] = value;
        i++;
    }
    
    return result;
}