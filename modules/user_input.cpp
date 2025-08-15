#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;

bool is_integer(const string&);
int iin(const string&);
bool is_float(const string&);
double fin(const string&);
string to_lower(string);
string to_upper(string);

int main() {
    double float_input = fin("Please key-in a valid float: ");

    cout << float_input << endl;
    return 0;
}

// integer input
int iin(const string& description) {
    string user_input;

    cout << description;
    cin >> user_input;

    if (is_integer(user_input)) {
        return stoi(user_input);
    } else {
        cout << "The input is not of type int." << endl;
        return iin(description);
    }
}

bool is_integer(const string& s) {
    for (char c : s) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

// float input
double fin(const string& description) {
    string user_input;

    cout << description;
    cin >> user_input;

    if (is_float(user_input)) {
        return stof(user_input);
    } else {
        cout << "The input is not of type float." << endl;
        return fin(description);
    }
}

bool is_float(const string& s) {
    for (char c : s) {
        if (!isdigit(c) && c != '.') {
            return false;
        }
    }
    return true;
}

string to_lower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);

    return s;
}

string to_upper(string s) {
    transform(s.begin(), s.end(), s.begin(), ::toupper);

    return s;
}