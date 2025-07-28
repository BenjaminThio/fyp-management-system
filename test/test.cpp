#include <string>
#include <iostream>
#include "uuid_v4.h"
using namespace std;

int main() {
    string uuid = UUIDv4::generate();
    cout << "Generated UUID: " << uuid << endl;
    return 0;
}
