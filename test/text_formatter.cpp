#include <iostream>
#include "text_formatter.h"
using namespace std;
using namespace ansi;

int main() {
    cout << format("Yen", FG_BLUE, BG_WHITE) << format("Zhang", UNDERLINE, BG_RED) << endl;
    return 0;
}
