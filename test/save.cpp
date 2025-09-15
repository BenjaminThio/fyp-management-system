#include <iostream>
#include <string>
#include <filesystem>
using namespace std;

namespace fs = filesystem;
const fs::path DESTINATION = "../data";
fs::path email = "benjaminthio@utarstudent.com";

void save(fs::path source);

int main() {
    save("C:/Users/User/Documents/hell/practice_1/snake.cpp");
    return 0;
}

void save(fs::path source) {
    if (!fs::exists(DESTINATION / email)) {
        fs::create_directories(DESTINATION / email);
    }

    fs::copy_file(source, DESTINATION / email / source.filename(), fs::copy_options::overwrite_existing);
}