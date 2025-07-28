#include <iostream>
#include <map>
#include <vector>
using namespace std;

map<string, map<string, string>> test_data_structure = {
    {
        // FYP title
        "12345-67890-12345", {
            {"title", "testing 123"},
            {"description", ""},
            {"uid", ""},
            {"date", ""},
            {"star", ""},
            {"registered_users", ""}
        }
    },
};

vector<string> get_keys(map<string, map<string, string>> data_map) {
    vector<string> keys;

    for (const auto& pair : data_map) {
        keys.push_back(pair.first);
    }

    return keys;
}

void render_titles() {
    vector<string> project_titles = get_keys(test_data_structure);

    cout << "Project Titles: " << endl;
    for (int project_index = 0 ; project_index < project_titles.size(); project_index++) {
        string project_title_uuid = project_titles[project_index];
        string project_title = test_data_structure[project_title_uuid]["title"];

        cout << project_index + 1 << ". " << project_title << endl;
    }
}

int main() {
    render_titles();

    return 0;
}