#include <iostream>
#include "text_formatter.h"
using namespace std;
using namespace ansi;

int main() {
    cout << format("Ng", FG_BLUE, UNDERLINE) << format("Yao Zong", BG_YELLOW) << endl;
    return 0;
}
