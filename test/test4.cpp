#include <string>
#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <cmath>
using namespace std;

auto round2 = [](double x) {
    return std::round(x * 100) / 100; // round to 2 decimals
};

// --- Utility: split string by delimiter ---
vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(str);
    while (getline(tokenStream, token, delimiter)) {
        if (!token.empty())
            tokens.push_back(token);
    }
    return tokens;
}

// --- Utility: lowercase + strip punctuation ---
string normalize(const string& s) {
    string out;
    for (char c : s) {
        if (isalnum((unsigned char)c)) // keep letters/numbers only
            out.push_back(tolower(c));
    }
    return out;
}

// --- Levenshtein Distance ---
int levenshtein(const string& s1, const string& s2) {
    int n = s1.size(), m = s2.size();
    vector<int> prev(m + 1), curr(m + 1);

    iota(prev.begin(), prev.end(), 0); // 0,1,2,3...

    for (int i = 1; i <= n; i++) {
        curr[0] = i;
        for (int j = 1; j <= m; j++) {
            if (s1[i-1] == s2[j-1])
                curr[j] = prev[j-1];
            else
                curr[j] = 1 + min({prev[j], curr[j-1], prev[j-1]});
        }
        swap(prev, curr);
    }
    return prev[m];
}

// --- Similarity Ratio ---
float similarity_ratio(const string& a, const string& b) {
    if (a.empty() && b.empty()) return 1.0f;
    int dist = levenshtein(a, b);
    return 1.0f - (float)dist / max(a.size(), b.size());
}

// --- Main function ---
bool similar(const string& text, const string& stringified_keywords) {
    char delimiter = ' ';
    vector<string> keywords = split(stringified_keywords, delimiter);
    vector<string> text_segments = split(text, delimiter);

    // Normalize all words
    for (auto& kw : keywords) kw = normalize(kw);
    for (auto& ts : text_segments) ts = normalize(ts);

    for (const auto& keyword : keywords) {
        for (const auto& segment : text_segments) {
            float sim = similarity_ratio(keyword, segment);

            cout << "Comparing \"" << keyword << "\" vs \"" << segment
                 << "\" → Similarity: " << sim * 100 << "%" << endl;
            
            if (round2(sim) >= 0.7) // threshold: 70% match
                return true;
        }
    }
    return false;
}

// --- Example usage ---
int main() {
    string text = "Developing of an IoT Based Smart Parking System.";
    string query = "Develop";

    if (similar(text, query))
        cout << "✅ Match found!" << endl;
    else
        cout << "❌ No match." << endl;

    return 0;
}