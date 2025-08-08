#ifndef DATABASE_H
#define DATABASE_H

#include "simdjson.h"
using namespace simdjson;

json load(const string& file_path);
json& save(const string& file_path, json& data);
json doc_to_json(ondemand::value val);

#endif