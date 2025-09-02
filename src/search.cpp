#include <string>
#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <cmath>
#include "utils.h"
using namespace std;

static auto round2 = [](double x) {
    return std::round(x * 100) / 100; // round to 2 decimals
};

// --- Utility: lowercase + strip punctuation ---
static string normalize(const string& s) {
    string out;
    for (char c : s) {
        if (isalnum((unsigned char)c)) // keep letters/numbers only
            out.push_back(tolower(c));
    }
    return out;
}

// --- Levenshtein Distance ---
static int levenshtein(const string& s1, const string& s2) {
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
static float similarity_ratio(const string& a, const string& b) {
    if (a.empty() && b.empty()) return 1.0f;
    int dist = levenshtein(a, b);
    return 1.0f - (float)dist / max(a.size(), b.size());
}

namespace search {
    // --- Main function ---
    bool similar(const string& text, const string& stringified_keywords) {
        char delimiter = ' ';
        vector<string> keywords = split_2(stringified_keywords, delimiter);
        vector<string> text_segments = split_2(text, delimiter);

        // Normalize all words
        for (auto& kw : keywords) kw = normalize(kw);
        for (auto& ts : text_segments) ts = normalize(ts);

        for (const auto& keyword : keywords) {
            for (const auto& segment : text_segments) {
                float sim = similarity_ratio(keyword, segment);

                /*
                cout << "Comparing \"" << keyword << "\" vs \"" << segment
                    << "\" → Similarity: " << sim * 100 << "%" << endl;
                */
                
                if (round2(sim) >= 0.7) // threshold: 70% match
                    return true;
            }
        }
        return false;
    }
}