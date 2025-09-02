#pragma once
#include <string>
using namespace std;

string render_big_text(string s);
void typing(const string& s, string& renderer, map<int, bool>& active_tasks, const int task);