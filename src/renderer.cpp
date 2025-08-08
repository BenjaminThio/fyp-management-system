#include <iostream>
#include <vector>
#include "globals.h"
#include "magic_enum.hpp"
#include "database.h"
#include "uuid_v4.h"
#include "simdjson.h"
#include "table.h"
#include "input.h"
using namespace std;
using namespace magic_enum;
using namespace simdjson;

void renderPage() {
    string renderer = "";

    system("cls");

    renderer += " ______   _______ \n|  _ \\ \\ / |  ___|\n| |_) \\ V /| |_   \n|  __/ | | |  _|  \n|_|    |_| |_|\n\n";

    switch (page) {
        case static_cast<int>(Page::MAIN): {
            for (int optionIndex = 0; optionIndex < enum_count<MainPageOption>(); optionIndex++) {
                optional<MainPageOption> option = enum_cast<MainPageOption>(optionIndex);

                if (option.has_value()) {
                    renderer += optionIndex == selectedOption ? '>' : ' ';
                    renderer += ' ';
                    renderer += enum_name<MainPageOption>(option.value());
                    renderer += '\n';
                } else {
                    cerr << "Option not found!" << endl;
                    getchar();
                    exit(0);
                }
            }
            break;
        }
        case static_cast<int>(Page::FYPS): {
            json fypTitles = load("test");

            for (int i = 0; i < 10; i++) {
                string uuid = UUIDv4::generate();

                renderer += (i == selectedOption ? '>' : ' ');
                renderer += ' ';
                renderer += uuid;
                renderer += '\n';
            }
            break;
        }
        case static_cast<int>(Page::CONSOLE): {
            json jsonData = load("test");
            vector<vector<string>> table = {
                {"No.", "Titles", "Public", "", "", ""}
            };
            
            for (int i = 0; i < jsonData.size(); i++) {
                vector<string> keys = jsonData.keys();
                vector<string> row = {to_string(i + 1) + ". ", keys[i], "ON", "View", "Edit", "Delete"};

                table.push_back(row);
            }

            renderer += generateTable(table);
            break;
        }
        case static_cast<int>(Page::SIGN_UP): {
            string userInput;

            renderer += "Username:\n\nPassword:\n\nEmail:";
            cout << renderer;
            setInputCursorPosition(0, 7);
            getline(cin, userInput);
            break;
        }
        default: {
            optional<Page> scopedPage = enum_cast<Page>(page);

            if (scopedPage.has_value()) {
                renderer += enum_name<Page>(scopedPage.value());
                renderer += " page is under developement.";
            } else {
                cerr << "Page not found!" << endl;
                getchar();
                exit(0);
            }
            break;
        }
    }

    cout << renderer;
}