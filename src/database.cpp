#include <iostream>
#include <filesystem>
#include <fstream>
#include "json.h"
#include "database.h"
#include "uuid_v4.h"
using namespace std;
using namespace filesystem;

json default_data = json::dictionary{
    {"Benjamin Thio Zi Liang", 1}
};

/*
int main() {
    json j;

    for (size_t i = 0; i < 10; i++) {
        string uuid = UUIDv4::generate();

        j[uuid] = json::dictionary{
            { "Benjamin", 1 }
        };
    }

    save("../data/test.json", j);

    cout << j.parse_string() << endl;
}

int main() {
    json test = json::dictionary{};

    test["TEST"] = "Ben";

    cout << test.parse_string() << endl;

    try {
        ondemand::parser parser;
        padded_string str_json = padded_string::load("../data/test.json");
        ondemand::document doc = parser.iterate(str_json);
        json j = doc_to_json(doc);

        cout << j.parse_string() << endl;
    } catch(const exception& e) {
        cerr << e.what() << endl;

        return -1;
    }

    return 0;
}
*/

json load(const string& file_path) {
    json j;

    if (exists(file_path)) {
        ondemand::parser parser;
        padded_string json = padded_string::load(file_path);
        ondemand::document doc = parser.iterate(json);

        j = doc_to_json(doc);
    } else j = save(file_path, default_data);

    return j;
}

json& save(const string& file_path, json& data) {
    ofstream outFile(file_path);

    if (outFile.is_open()) {
        outFile << data.parse_string() << endl;
        outFile.close();
    } else cerr << "Error opening file!" << endl;

    return data;
}

json doc_to_json(ondemand::value val) {
    auto type = val.type();

    if (type.error()) throw runtime_error("Not a valid type.");

    switch(type.value()) {
        case ondemand::json_type::null: return nullptr;
        case ondemand::json_type::boolean: {
            auto b = val.get_bool();

            if (b.error()) throw runtime_error("Not a valid boolean type.");

            return json(b.value());
        }
        case ondemand::json_type::number: {
            auto n = val.get_int64();

            if (!n.error()) return json(n.value());

            auto d = val.get_double();

            if (!d.error()) return json(d.value());

            throw runtime_error("Not a valid integer/double type.");
        }
        case ondemand::json_type::string: {
            auto s = val.get_string();

            if (s.error()) throw runtime_error("Not a valid string type.");

            return json(string(s.value()));
        }
        case ondemand::json_type::array: {
            auto arr = val.get_array();

            if (arr.error()) throw runtime_error("Not a valid array type.");

            json::list l;

            for (auto elem : arr) {
                l.push_back(doc_to_json(elem.value()));
            }

            return json(l);
        }
        case ondemand::json_type::object: {
            auto obj = val.get_object();

            if (obj.error()) throw runtime_error("Not a valid object.");

            json::dictionary d;

            for (auto field : obj.value()) {
                auto key_result = field.unescaped_key();

                if (key_result.error()) {
                    runtime_error("Not a valid key.");
                    continue;
                }

                string key(key_result.value());

                d[key] = doc_to_json(field.value());
            }

            return json(d);
        }
        default: throw runtime_error("Unknown JSON type.");
    }
}