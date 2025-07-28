#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <sstream>
#include <variant>
#include <string>
#include <vector>
#include <map>
#include "simdjson.h"
using namespace std;
using namespace simdjson;

class json {
public:
    using array = std::vector<json>;
    using object = std::map<std::string, json>;

private:
    std::variant<
        std::nullptr_t,
        std::string,
        int,
        double,
        bool,
        array,
        object
    > value;

public:
    json() : value(nullptr) {}
    json(const char* s) : value(std::string(s)) {}
    json(const std::string& s) : value(s) {}
    json(int i) : value(i) {}
    json(double d) : value(d) {}
    json(bool b) : value(b) {}
    json(const array& a) : value(a) {}
    json(const object& o) : value(o) {}
    json(std::initializer_list<json> a) : value(array(a)) {}
    json(std::initializer_list<std::pair<const std::string, json>> o)
        : value(object(o)) {}

    bool is_null()   const { return std::holds_alternative<std::nullptr_t>(value); }
    bool is_bool()   const { return std::holds_alternative<bool>(value); }
    bool is_int()    const { return std::holds_alternative<int>(value); }
    bool is_double() const { return std::holds_alternative<double>(value); }
    bool is_string() const { return std::holds_alternative<std::string>(value); }
    bool is_array()  const { return std::holds_alternative<array>(value); }
    bool is_object() const { return std::holds_alternative<object>(value); }

    json& operator[](const std::string& key) {
        if (!is_object()) value = object();
        return std::get<object>(value)[key];
    }

    const json& operator[](const std::string& key) const {
        static json null_json;
        if (is_object()) {
            const auto& obj = std::get<object>(value);
            auto it = obj.find(key);
            return it != obj.end() ? it->second : null_json;
        }
        return null_json;
    }

    json& at(size_t index) {
        return std::get<array>(value).at(index);
    }

    const json& at(size_t index) const {
        return std::get<array>(value).at(index);
    }

    void push_back(const json& j) {
        if (!is_array()) value = array();
        std::get<array>(value).push_back(j);
    }

    auto begin() -> array::iterator {
        return std::get<array>(value).begin();
    }

    auto end() -> array::iterator {
        return std::get<array>(value).end();
    }

    auto begin() const -> object::const_iterator {
        return std::get<object>(value).begin();
    }

    auto end() const -> object::const_iterator {
        return std::get<object>(value).end();
    }

    std::string parse_string(int indent = 4) const {
        std::ostringstream oss;
        serialize(value, oss, indent, 0);
        return oss.str();
    }

    friend std::ostream& operator<<(std::ostream& os, const json& j) {
        return os << j.parse_string(2);
    }

private:
    static void serialize(const std::variant<
        std::nullptr_t, std::string, int, double, bool, array, object>& v,
        std::ostringstream& oss, int indent, int depth) {

        auto indent_str = [&](int d) { return std::string(d * indent, ' '); };

        if (std::holds_alternative<std::nullptr_t>(v)) {
            oss << "null";
        } else if (std::holds_alternative<std::string>(v)) {
            oss << "\"" << escape(std::get<std::string>(v)) << "\"";
        } else if (std::holds_alternative<int>(v)) {
            oss << std::get<int>(v);
        } else if (std::holds_alternative<double>(v)) {
            oss << std::get<double>(v);
        } else if (std::holds_alternative<bool>(v)) {
            oss << (std::get<bool>(v) ? "true" : "false");
        } else if (std::holds_alternative<array>(v)) {
            const auto& arr = std::get<array>(v);
            if (arr.empty()) {
                oss << "[]";
                return;
            }
            oss << "[\n";
            for (size_t i = 0; i < arr.size(); ++i) {
                oss << indent_str(depth + 1);
                serialize(arr[i].value, oss, indent, depth + 1);
                if (i + 1 < arr.size()) oss << ",";
                oss << "\n";
            }
            oss << indent_str(depth) << "]";
        } else if (std::holds_alternative<object>(v)) {
            const auto& obj = std::get<object>(v);
            if (obj.empty()) {
                oss << "{}";
                return;
            }
            oss << "{\n";
            size_t count = 0;
            for (const auto& [k, val] : obj) {
                oss << indent_str(depth + 1) << "\"" << escape(k) << "\": ";
                serialize(val.value, oss, indent, depth + 1);
                if (++count < obj.size()) oss << ",";
                oss << "\n";
            }
            oss << indent_str(depth) << "}";
        }
    }

    static std::string escape(const std::string& s) {
        std::ostringstream out;
        for (char c : s) {
            switch (c) {
                case '"': out << "\\\""; break;
                case '\\': out << "\\\\"; break;
                case '\b': out << "\\b"; break;
                case '\f': out << "\\f"; break;
                case '\n': out << "\\n"; break;
                case '\r': out << "\\r"; break;
                case '\t': out << "\\t"; break;
                default:
                    if (static_cast<unsigned char>(c) < 0x20) {
                        char buf[7];
                        snprintf(buf, sizeof(buf), "\\u%04x", c);
                        out << buf;
                    } else {
                        out << c;
                    }
            }
        }
        return out.str();
    }
};

json load(const string&);
void save(const string&, json);
json docTojson(ondemand::document&);
json convert_to_json(ondemand::value val);

#endif