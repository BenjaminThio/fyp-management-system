#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

template <typename T>
vector<vector<T>> group(vector<T> elements, size_t step = 1) {
    vector<vector<T>> result = { {} };
    int i = 0;

    for (const auto& element : elements) {
        if (i == step) {
            i = 0;
            result.push_back({});
        }
        result[result.size() - 1].push_back(element);
        i++;
    }

    return result;
}

int main() {
    ostringstream oss;
    vector<int> elements = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
    auto grp = group(elements, 3);

    oss << '[';
    for (size_t j = 0; j < grp.size(); j++) {
        oss << '[';
        for (size_t i = 0; i < grp[j].size(); i++)
            oss << grp[j][i] << (i < grp[j].size() - 1 ? ", " : "");
        oss << ']' << (j < grp.size() - 1 ? ", " : "");
    }
    oss << ']';

    cout << oss.str() << endl;
    return 0;
}