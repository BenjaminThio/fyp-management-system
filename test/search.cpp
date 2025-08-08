#include <iostream>
#include <vector>
using namespace std;

vector<string> split(string text, char delimiter);

int main() {
    string text = "Benjamin Thio is a gay.";
    string stringified_keywords = "Benjimin";
    char delimiter = ' ';
    vector<string> keywords = split(stringified_keywords, delimiter);
    vector<string> text_segments = split(text, delimiter);

    for (int keyword_index = 0; keyword_index < keywords.size(); keyword_index++) {
        string keyword = keywords[keyword_index];

        for (int text_segment_index = 0; text_segment_index < text_segments.size(); text_segment_index++) {
            string text_segment = text_segments[text_segment_index];
            float identical_counter = 0;
            float identical_streak_counter = 0;
            float highest_identical_streaks = 0;

            for (int keyword_char_index = 0; keyword_char_index < keyword.length(); keyword_char_index++) {
                if (keyword_char_index <= text_segment.length() -1 && keyword[keyword_char_index] == text_segment[keyword_char_index]) {
                    identical_counter++;
                    identical_streak_counter++;
                }
                else {
                    if (identical_streak_counter > highest_identical_streaks) {
                        highest_identical_streaks = identical_streak_counter;
                    }

                    identical_streak_counter = 0;
                }
            }
            float similarity = identical_counter / text_segment.length() * 100;
            float partial_similarity = identical_counter / keyword.length() * 100;

            cout << "Similarity: " << similarity << "%, Partial similarity: " << partial_similarity << "%, Highest Streak: " << highest_identical_streaks << '/' << keyword.length() << endl;  
        }
    }
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
