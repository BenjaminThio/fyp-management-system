#include <iostream>
#include <string>
#include <vector>
#include <cmath>
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

double round_sf(double value, int sf) {
    int whole_number_length = split(to_string(value), '.')[0].length();
    double decimal_point_offset = pow(10, sf - whole_number_length);
    double rounded_value = round(value * decimal_point_offset) / decimal_point_offset;

    return rounded_value;
}

string stringify_rounded_dec(double value, int dec) {
    vector<string> decimal_number_parts = split(to_string(value), '.');
    string whole_number = decimal_number_parts[0];
    string fractional_part = to_string(stoi(decimal_number_parts[1]));
    int fractional_part_length = fractional_part.length();
    double decimal_point_offset = pow(10, dec - fractional_part_length);
    double rounded_value = round(stoi(fractional_part) * decimal_point_offset);
    string stringified_rounded_decimal_number = whole_number + "." + split(to_string(rounded_value), '.')[0];

    return stringified_rounded_decimal_number;
}

double round_dec(double value, int dec) {
    return stod(stringify_rounded_dec(value, dec));
}

int main() {
    cout << stringify_rounded_dec(1.375, 3) << endl;

    return 0;
}