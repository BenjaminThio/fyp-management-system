#include <iostream>
#include <string>
#include <utility>
using namespace std;

int main() {
    int amount;
    pair<string, int> ringgits[] = {
        {"100", 0},
        {"50", 0},
        {"10", 0},
        {"5", 0},
        {"1", 0}
    };

    cout << "Please key-in an amount: ";
    cin >> amount;

    for (int i = 0; i < sizeof(ringgits) / sizeof(*ringgits); i++) {
        string key = ringgits[i].first;
        float ringgit = stof(key);
        
        if (ringgit > 1) {
            if (amount / ringgit > 1) {
                ringgits[i].second = int(amount / ringgit);
                amount -= int(amount / ringgit) * ringgit;
            }
        }
        else {
            ringgits[i].second = amount;
        }
    }

    cout << "RM 100: " << ringgits[0].second << ", RM 50: " << ringgits[1].second << ", RM 10: " << ringgits[2].second << ", RM 5: " << ringgits[3].second << ", RM 1: " << ringgits[4].second;
}

