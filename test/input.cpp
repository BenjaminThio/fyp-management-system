#include <iostream>
#include <string>
using namespace std;

int iin(const string&);
bool isInteger(const string&);

int main() {
    iin("Please key-in a valid integer: ");
    return 0;
}

int iin(const string& description) {
    string userInput;

    cout << description;
    cin >> userInput;

    if (isInteger(userInput)) {
        return stoi(userInput);
    } else {
        cout << "The input provided is not type of integer." << endl;
        return iin(description);
    }
}

bool isInteger(const string& s) {
    for (char c : s) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}