#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <thread>
#include <chrono>
#include <array>
#include <functional>
#include "renderer.h"
#include "utils.h"
using namespace std;

static map<char, string> big_chars = {
    { 'A', "    \n /\\ \n/--\\" },
    { 'B', " __ \n|__)\n|__)" },
    { 'C', " __\n/  \n\\__" },
    { 'D', " __ \n|  \\\n|__/" },
    { 'E', " __\n|_ \n|__" },
    { 'F', " __\n|_ \n|  " },
    { 'G', " __ \n/ _ \n\\__)" },
    { 'H', "    \n|__|\n|  |" },
    { 'I', " \n|\n|" },
    { 'J', " __\n  |\n\\_)" },
    { 'K', "   \n|_/\n| \\" },
    { 'L', "   \n|  \n|__" },
    { 'M', "    \n|\\/|\n|  |" },
    { 'N', "    \n|\\ |\n| \\|" },
    { 'O', " __ \n/  \\\n\\__/" },
    { 'P', " __ \n|__)\n|   " },
    { 'Q', " __ \n/  \\\n\\_\\/" },
    { 'R', " __ \n|__)\n| \\ " },
    { 'S', " __\n(_ \n__)" },
    { 'T', "___\n | \n | " },
    { 'U', "    \n/  \\\n\\__/" },
    { 'V', "    \n\\  /\n \\/ " },
    { 'W', "    \n|  |\n|/\\|" },
    { 'X', "   \n\\_/\n/ \\" },
    { 'Y', "   \n\\_/\n | " },
    { 'Z', "___\n _/\n/__" },
    { ' ', "  \n  \n  " },
    { '|', "|\n|\n|" },
    { '.', " \n \n." },
    { ',', " \n \n," }
};

string render_big_text(string s) {
    array<string, 3> lines;

    for (const char c : s) {
        vector<string> split_newline = split(big_chars[c], '\n');

        lines[0] += split_newline[0];
        lines[1] += split_newline[1];
        lines[2] += split_newline[2];
    }

    return lines[0] + "\n" + lines[1] + "\n" + lines[2];
}

void typing(const string& s, string& renderer, map<int, bool>& active_tasks, const int id) {
    bool is_caret_visible = false;

    for (size_t i = 1; i < s.size(); i++) {
        system("cls");
        if (active_tasks[id]) {
            renderer = render_big_text(s.substr(0, i) + "|");
            render_page();
        } else return;
        this_thread::sleep_for(chrono::milliseconds(50));
        if (!active_tasks[id]) return;
    }
    
    while (active_tasks[id]) {
        system("cls");
        renderer = render_big_text(s + (is_caret_visible ? "|" : ""));
        render_page();
        this_thread::sleep_for(chrono::milliseconds(500));
        if (!active_tasks[id]) return;
        is_caret_visible = !is_caret_visible;
    }

    active_tasks.erase(id);
}