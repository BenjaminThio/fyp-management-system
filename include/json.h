#ifndef JSON_H
#define JSON_H

#include <cstdint>
#include <vector>
#include <map>
#include <variant>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
using namespace std;

class json {
    public:
        using list = vector<json>;
        using dictionary = map<string, json>;
    private:
        variant<nullptr_t, bool, int, int64_t, double, string, list, dictionary> value;
    public:
        json() : value(nullptr) {}
        json(nullptr_t) : value(nullptr) {}
        json(bool b) : value(b) {}
        json(int i) : value(static_cast<int64_t>(i)) {}
        json(int64_t i) : value(i) {}
        json(double d) : value(d) {}
        json(const char* s) : value(string(s)) {}
        json(const string& s) : value(s) {}
        json(const list& l) : value(l) {}
        json(const dictionary& d) : value(d) {}

        bool is_null() const { return holds_alternative<nullptr_t>(value); }
        bool is_bool() const { return holds_alternative<bool>(value); }
        bool is_int() const { return holds_alternative<int64_t>(value); }
        bool is_float() const { return holds_alternative<double>(value); }
        bool is_string() const { return holds_alternative<string>(value); }
        bool is_list() const { return holds_alternative<list>(value); }
        bool is_dictionary() const { return holds_alternative<dictionary>(value); }

        bool contains(const json& target) {
            if (is_list()) {
                list l = get<list>(value);

                if (find(l.begin(), l.end(), target) != l.end()) return true;
                else return false;
            } else if (is_dictionary()) {
                if (!target.is_string()) throw runtime_error("Not a string!");

                dictionary d = get<dictionary>(value);

                return d.find(get<string>(target.value)) != d.end();
            } else throw runtime_error("Not a list/dictionary.");
        }

        vector<string> keys() const {
            if (is_dictionary()) {
                vector<string> result;

                for (const pair<string, json> p : get<dictionary>(value)) result.push_back(p.first);

                return result;
            }
            else throw runtime_error("Not a dictionary!");
        }

        size_t size() const {
            if (is_list()) return get<list>(value).size();
            else if (is_dictionary()) return get<dictionary>(value).size();
            else throw runtime_error("Not a list/dictionary.");

            return 0;
        }

        json& operator[](const size_t& idx) {
            if (!is_list()) throw runtime_error("Not a list!");
            else return get<list>(value)[idx];
        }

        json& operator[](const string& key) {
            if (!is_dictionary()) throw runtime_error("Not a dictionary"); //*this = dictionary{};
            
            return get<dictionary>(value)[key];
        }

        json& operator[](const char* key) {
            if (!is_dictionary()) throw runtime_error("Not a dictionary"); //*this = dictionary{};

            return (*this)[string(key)];
        }

        operator bool() const {
            if (is_bool()) return get<bool>(value); else throw runtime_error("Not a boolean!");
        }
        
        bool operator==(const nullptr_t) const { return is_null(); }
        bool operator!=(const nullptr_t) const { return !is_null(); }
        bool operator==(const bool b) const { return is_bool() && get<bool>(value) == b; }
        bool operator!=(const bool b) const { return !is_bool() || get<bool>(value) != b; }
        bool operator==(const int64_t i) const { return is_int() && get<int64_t>(value) == i; }
        bool operator!=(const int64_t i) const { return !is_int() || get<int64_t>(value) != i; }
        bool operator==(const double d) const { return is_float() && get<double>(value) == d; }
        bool operator!=(const double d) const { return !is_float() || get<double>(value) != d; }
        bool operator==(const string s) const { return is_string() && get<string>(value) == s; }
        bool operator!=(const string s) const { return !is_string() || get<string>(value) != s; }
        bool operator==(const list l) const { return is_list() && get<list>(value) == l; }
        bool operator!=(const list l) const { return !is_list() || get<list>(value) != l; }
        bool operator==(const dictionary d) const { return is_dictionary() && get<dictionary>(value) == d; }
        bool operator!=(const dictionary d) const { return !is_dictionary() || get<dictionary>(value) != d; }
        bool operator==(const json& j) const { return value == j.value; }
        bool operator!=(const json& j) const { return value != j.value; }

        list& as_list() {
            if (!is_list()) throw runtime_error("Not a dictionary!");

            return get<list>(value);
        }

        dictionary& as_dictionary() {
            if (!is_dictionary()) throw runtime_error("Not a dictionary!");

            return get<dictionary>(value);
        }

        string& as_string() {
            if (!is_string()) throw runtime_error("Not a string!");

            return get<string>(value);
        }

        string parse_string(const size_t indent = 0, const bool left_trim = false) const {
            ostringstream oss;
            string indent_str(indent, ' ');

            if (!left_trim) oss << indent_str;
            if (holds_alternative<nullptr_t>(value)) oss << "null";
            else if (holds_alternative<bool>(value)) oss << boolalpha << get<bool>(value);
            else if (holds_alternative<int64_t>(value)) oss << get<int64_t>(value);
            else if (holds_alternative<double>(value)) oss << fixed << setprecision(6) << get<double>(value);
            else if (holds_alternative<string>(value)) oss << '"' << get<string>(value) << '"';
            else if (holds_alternative<list>(value)) {
                const list l = get<list>(value);

                if (l.size() > 0) {
                    oss << '[' << endl;
                    for (size_t i = 0; i < l.size(); i++) oss << l[i].parse_string(indent + 4) << ((i < l.size() - 1) ? "," : "") << endl;
                    oss << indent_str << ']';
                } else oss << "[]" << endl;
            } else if (holds_alternative<dictionary>(value)) {
                dictionary d = get<dictionary>(value);
                size_t counter = 0;

                if (d.size() > 0) {
                    oss << '{' << endl;
                    for (const auto& [key, val] : d) oss << indent_str << string(4, ' ') + '"' << key << '"' << ": " << val.parse_string(indent + 4, true) << ((++counter < d.size()) ? "," : "") << endl;
                    oss << indent_str << '}';
                } else oss << "{}" << endl;
                /*
                dictionary d = get<dictionary>(value);
                vector<string> d_keys = keys();

                oss << '{' << endl;
                for (size_t i = 0; i < d.size(); i++) {
                    string key = d_keys[i];
                    json val = d[key];

                    oss << indent_str << string(4, ' ') + '"' << key << '"' << ": " << val.parse_string(indent + 4, true) << ((i < d.size() - 1) ? "," : "") << endl;
                }
                oss << indent_str << '}';
                */
            }

            return oss.str();
        }
};

#endif