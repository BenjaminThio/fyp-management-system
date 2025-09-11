#pragma once

#include <vector>
#include <string>
#include "json.h"
using namespace std;

vector<string> split(string text, char delimiter);
vector<string> split_2(const string& str, char delimiter);
template <typename T>
vector<vector<T>> group(vector<T> elements, size_t step = 1);
vector<json::dictionary> group_dict(json dict, size_t step = 1);