#include <iostream>
#include <regex>
using namespace std;

int main() {
    regex password_pattern(R"(^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[~`!@#$%^&*()_\-+={[}\]|\\:;"'<,>.?\/])[^\s]{8,32}$)");
                        
    if (!regex_match("bnm", password_pattern)) {
        cout << "TEST" << endl;
    }
    return 0;
}