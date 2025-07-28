#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <variant>
#include <vector>
#include <map>
#include "simdjson.h"
#include "uuid_v4.h"
#include "database.h"
using namespace std;
using namespace simdjson;
using namespace filesystem;

int main() {
    json jdata;

    for (int i = 0; i < 10; i++) {
        string uuid = UUIDv4::generate();
        cout << uuid << endl;

        jdata[uuid] = json::object{
            pair<string, json>{"abc", json::array{2, 3, 4, 5}}
        };
    }
    save("test", jdata);
    return 0;
}

json load(const string& filename) {
    string filePath = "../data/" + filename + ".json";
    json j;

    if (exists(filePath)) {
        ondemand::parser parser;
        padded_string json = padded_string::load(filePath);
        ondemand::document doc = parser.iterate(json);
        j = docTojson(doc);
    }
    else {
        // save(filename);
        cerr << "File not found!" << endl;
        getchar();
        exit(0);
    }

    return j;
}

json docTojson(ondemand::document& doc) {
    ondemand::object obj = doc.get_object();
    json j;

    for (auto field : doc.get_object()) {
        string_view key_view;
        auto error = field.unescaped_key().get(key_view);
        if (error) {
            cerr << "ERROR!" << endl;
            continue;
        }
        string key(key_view);
        j[key] = convert_to_json(field.value());
    }

    return j;
}

json convert_to_json(ondemand::value val) {
    if (val.type() == ondemand::json_type::string) {
        string_view str;
        if (auto err = val.get(str); !err) return json(string(str));
    } 
    else if (val.type() == ondemand::json_type::boolean) {
        bool b;
        if (auto err = val.get(b); !err) return json(b);
    }
    else if (val.type() == ondemand::json_type::number) {
        int64_t i;
        if (val.get(i) == SUCCESS) return json(static_cast<int>(i));
        double d;
        if (val.get(d) == SUCCESS) return json(d);
    } 
    else if (val.type() == ondemand::json_type::object) {
        ondemand::object obj;
        if (val.get(obj) == SUCCESS) {
            json result;
            for (auto field : obj) {
                string_view key = field.unescaped_key();
                result[string(key)] = convert_to_json(field.value());
            }
            return result;
        }
    }
    else if (val.type() == ondemand::json_type::array) {
        ondemand::array arr;
        if (val.get(arr) == SUCCESS) {
            vector<json> items;
            for (auto element_result : arr) {
                ondemand::value element;
                if (auto error = element_result.get(element)) {
                    continue;
                }
                items.push_back(convert_to_json(element));
            }
            return json(items);
        }
    }

    return json();
}

void save(const string& filename, json data) {
    string filePath = "../data/" + filename + ".json";
    ofstream outFile(filePath);

    if (outFile.is_open()) {
        outFile << data.parse_string() << endl;
        outFile.close();
    } else {
        cerr << "Error opening file!" << endl;
    }
}