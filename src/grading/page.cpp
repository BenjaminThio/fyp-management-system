#include <sstream>
#include <conio.h>
#include <array>
#include <vector>
#include <iomanip>
#include "keyboard.h"
#include "globals.h"
#include "renderer.h"
#include "audio.h"
#include "table.h"
#include "text_formatter.h"
#include "input.h"
#include "terminal.h"
#include "view_submission/page.h"
using namespace std;
using namespace ansi;

namespace grading {
    struct input_header : globals::input_header {
        size_t max_value;

        input_header(string field, int local_caret_pos, int input_field_view_offset, size_t min_length, size_t max_length, int length, bool error, size_t max_value)
        :
        globals::input_header(field, local_caret_pos, input_field_view_offset, min_length, max_length, length, error), max_value(max_value) {};
    };
    static coord_3d field_coord;
    static bool focus = false;
    static array<vector<vector<string>>, 2> tables = {
        vector<vector<string>>{
            { "Component", "Supervisor", "Moderator", "Average" },
            { "FYP 1 Report (15%)" },
            { "FYP 1 Presentation (10%)" },
            { "FYP 1 Effort (5%)" }
        },
        vector<vector<string>>{
            { "Component", "Supervisor", "Moderator", "Average" },
            { "Final Report (20%)" },
            { "Product Demo (35%)" },
            { "FYP 2 Presentation (10%)" },
            { "FYP 2 Effort (5%)" }
        }
    };
    static array<vector<vector<input_header>>, 4> inputs = {
        vector<vector<input_header>>{
            { input_header("View FYP 1", 0, 0, 0, 10, 10, false, 0) }
        },
        vector<vector<input_header>>{
            { input_header("0", 1, 0, 0, 2, 2, false, 15), input_header("0", 1, 0, 0, 2, 2, false, 15) },
            { input_header("0", 1, 0, 0, 2, 2, false, 10), input_header("0", 1, 0, 0, 2, 2, false, 10) },
            { input_header("0", 1, 0, 0, 1, 1, false, 5), input_header("0", 1, 0, 0, 1, 1, false, 5) }
        },
        vector<vector<input_header>>{
            { input_header("View FYP 2", 1, 0, 0, 10, 10, false, 0) }
        },
        vector<vector<input_header>>{
            { input_header("0", 1, 0, 0, 2, 2, false, 20), input_header("0", 1, 0, 0, 2, 2, false, 20) },
            { input_header("0", 1, 0, 0, 2, 2, false, 35), input_header("0", 1, 0, 0, 2, 2, false, 35) },
            { input_header("0", 1, 0, 0, 2, 2, false, 10), input_header("0", 1, 0, 0, 2, 2, false, 10) },
            { input_header("0", 1, 0, 0, 1, 1, false, 5), input_header("0", 1, 0, 0, 1, 1, false, 5) }
        }
    };

    static string get_class_number(double cgpa) {
        if (cgpa < 0 || cgpa > 4) throw runtime_error("Not a valid cpga.");

        if (cgpa >= 3.5 && cgpa <= 4) return "First";
        else if (cgpa >= 3.00 && cgpa <= 3.49) return "Upper Second";
        else if (cgpa >= 2.20 && cgpa <= 2.99) return "Lower Second";
        else if (cgpa >= 2.00 && cgpa <= 2.19) return "Third";
        else return "Fail";
    }

    static double marks_to_cgpa(double marks) {
        if (marks < 0 || marks > 100) throw runtime_error("Not a valid mark.");

        if (marks >= 80 && marks <= 89.99 || marks >= 90 && marks <= 100) return 4.0;
        else if (marks >= 75 && marks <= 79.99) return 3.67;
        else if (marks >= 70 && marks <= 74.99) return 3.33;
        else if (marks >= 65 && marks <= 69.99) return 3.0;
        else if (marks >= 60 && marks <= 64.99) return 2.67;
        else if (marks >= 55 && marks <= 59.99) return 2.33;
        else if (marks >= 50 && marks <= 54.99) return 2.0;
        else return 0.0;
    }
    
    void start() {
        for (size_t z = 0; z < inputs.size(); z++) {
            if (z == 0 || z == 2) continue;
            for (size_t y = 0; y < inputs[z].size(); y++) {
                int normalized_z = 0;

                switch (z) {
                    case 1:
                        normalized_z = 0;
                        break;
                    case 3:
                        normalized_z = 1;
                        break;
                }
                for (auto& header : inputs[z][y]) {
                    // tables[z][y + 1].push_back("TESTING123");
                    tables[normalized_z][y + 1].push_back(render_input_field(header.field, header.local_caret_pos, header.input_field_view_offset, header.length) + "%");
                }
                tables[normalized_z][y + 1].push_back(to_string((stod(inputs[z][y][0].field) + stod(inputs[z][y][1].field)) / 2) + "%");
            }
        }
    }

    static void generate_graph() {

    }

    static void caret_handler() {
        focus && field_coord.z < inputs.size() && field_coord.z != 0 && field_coord.z != 2 ? terminal::show_cursor() : terminal::hide_cursor();
    }

    static pair<string, size_t> next_utf8_char(const string& input, size_t raw_pos) {
        unsigned char c = input[raw_pos];
        size_t len = 1;

        if ((c & 0x80) == 0x00) {
            len = 1; // ASCII
        } else if ((c & 0xE0) == 0xC0) {
            len = 2;
        } else if ((c & 0xF0) == 0xE0) {
            len = 3;
        } else if ((c & 0xF8) == 0xF0) {
            len = 4;
        } else {
            throw runtime_error("Invalid UTF-8 sequence");
        }

        return {input.substr(raw_pos, len), raw_pos + len};
    }

    static string strip_ansi(const string& s) {
        string out;
        size_t i = 0;

        while (i < s.size()) {
            if (s[i] == '\x1b') {
                i++;
                if (i < s.size() && s[i] == '[') {
                    i++;
                    while (i < s.size() && (s[i] < 0x40 || s[i] > 0x7E)) i++;
                    if (i < s.size()) i++;
                }
            } else {
                auto [ch, next_pos] = next_utf8_char(s, i);
                out += ch;
                i = next_pos;
            }
        }
        return out;
    }

    void push_frame(ostringstream& renderer, array<int, 2>& manual_cursor_input_pos) {
        double total_marks = 0;

        caret_handler();

        for (size_t z = 0; z < inputs.size(); z++) {
            if (z == 0 || z == 2) continue;
            for (size_t y = 0; y < inputs[z].size(); y++) {
                ostringstream oss; 
                double score = (stod(inputs[z][y][0].field) + stod(inputs[z][y][1].field)) / 2;
                int normalized_z = 0;

                switch (z) {
                    case 1:
                        normalized_z = 0;
                        break;
                    case 3:
                        normalized_z = 1;
                        break;
                }

                for (size_t x = 0; x < inputs[z][y].size(); x++) {
                    input_header header = inputs[z][y][x];

                    tables[normalized_z][y + 1][x + 1] = render_input_field(
                        header.field,
                        header.local_caret_pos,
                        header.input_field_view_offset,
                        header.length,
                        (focus ? (field_coord.z == z && field_coord.y == y && field_coord.x == x ? BG_CYAN : BG_RED) : (field_coord.z == z && field_coord.y == y && field_coord.x == x ? BG_LIGHT_GREEN : BG_WHITE))) + "%";
                }

                oss << fixed << setprecision(2) << score;
                total_marks += score;
                tables[normalized_z][y + 1][3] = oss.str() + "%";
            }
        }

        double cgpa = marks_to_cgpa(total_marks);
        renderer
        << (
        view_submission::get_submitter().contains(json("fyp_1_id"))
        ?
        "Title: " + fyps[view_submission::get_submitter()["fyp_1_id"].as_string()]["info"]["name"].as_string()
        + "\nID: " + view_submission::get_submitter()["fyp_1_id"].as_string() + "\n"
        + render_static_input_field(inputs[0][0][0].field, inputs[0][0][0].length, (field_coord.z == 0 ? BG_GREEN : BG_WHITE)) + (field_coord.z == 0 ? " 👁" : "") + "\n"
        + generate_table(tables[0])
        :
        format(strip_ansi("Title: None\nID: None\n" + render_static_input_field(inputs[0][0][0].field, inputs[0][0][0].length, (field_coord.z == 0 ? BG_GREEN : BG_WHITE)) + (field_coord.z == 0 ? " 👁" : "") + "\n" + generate_table(tables[0])), FG_DARK_GRAY)
        ) << endl
        << (
        view_submission::get_submitter().contains(json("fyp_2_id"))
        ?
        "Title: " + fyps[view_submission::get_submitter()["fyp_2_id"].as_string()]["info"]["name"].as_string()
        + "\nID: " + view_submission::get_submitter()["fyp_2_id"].as_string() + "\n"
        + render_static_input_field(inputs[2][0][0].field, inputs[2][0][0].length, (field_coord.z == 2 ? BG_GREEN : BG_WHITE)) + (field_coord.z == 2 ? " 👁" : "") + "\n"
        + generate_table(tables[1])
        :
        format(strip_ansi("Title: None\nID: None\n" + render_static_input_field(inputs[2][0][0].field, inputs[2][0][0].length, (field_coord.z == 2 ? BG_GREEN : BG_WHITE)) + (field_coord.z == 2 ? " 👁" : "") + "\n" + generate_table(tables[1])), FG_DARK_GRAY)
        ) << endl
        // << '(' << field_coord.x << ',' << field_coord.y << ',' << field_coord.z << ')' << endl
        << "Final Marks: " << total_marks << '%' << endl
        << "CGPA: " << fixed << setprecision(2) << cgpa << endl
        << "Class: " << get_class_number(cgpa) << endl
        << endl
        << (field_coord.z == inputs.size() ? format("Save", UNDERLINE) : "Save");

        if (focus && field_coord.z < inputs.size() && field_coord.z != 0 && field_coord.z != 2)
            manual_cursor_input_pos = { 26 + (field_coord.x * 11) + inputs[field_coord.z][field_coord.y][field_coord.x].local_caret_pos, 9 + (field_coord.y * 2) + (field_coord.z * 10) };
    }

    void keyboard_input_callback() {
        int key = -1;
        int special_key = -1;

        if (focus && field_coord.z < inputs.size() && field_coord.z != 0 && field_coord.z != 2) {
            input_header& input = inputs[field_coord.z][field_coord.y][field_coord.x];
            array<int, 2> keyboard_input = int_input_field(input.field, input.local_caret_pos, input.input_field_view_offset, input.max_length, input.length, input.max_value);
            key = keyboard_input[0];
            special_key = keyboard_input[1];
        } else if (_kbhit()) {
            key = _getch();

            switch (key) {
                case 0:
                case 224:
                    special_key = _getch();
                    break;
            }
        }

        switch (key) {
            case static_cast<int>(Key::ENTER):
                if (field_coord.z < inputs.size() && field_coord.z != 0 && field_coord.z != 2) {
                    focus = !focus;
                } else {
                    // TODO: Save student's grade.
                }
                
                play_sound("select");
                render_page();
                break;
            case static_cast<int>(Key::ESCAPE):
                return_page();
                break;
        }

        if (!focus) {
            switch (special_key) {
                case static_cast<int>(Key::UP):
                case static_cast<int>(Key::DOWN):
                case static_cast<int>(Key::LEFT):
                case static_cast<int>(Key::RIGHT):
                    switch (special_key) {
                        case static_cast<int>(Key::UP):
                            if (field_coord.y - 1 >= 0) {
                                field_coord.y -= 1;
                            } else {
                                if (field_coord.z - 1 >= 0) {
                                    field_coord.z -= 1;
                                } else {
                                    field_coord.z = inputs.size(); // inputs.size() - 1;
                                }

                                if (field_coord.z < inputs.size()) {
                                    field_coord.y = inputs[field_coord.z].size() - 1;
                                }
                            }
                            break;
                        case static_cast<int>(Key::DOWN):
                            if (field_coord.z < inputs.size() && field_coord.y + 1 < inputs[field_coord.z].size()) {
                                field_coord.y += 1;
                            } else {
                                field_coord.y = 0;
                                if (field_coord.z + 1 <= inputs.size()) { // < inputs.size()
                                    field_coord.z += 1;
                                } else {
                                    field_coord.z = 0;
                                }
                            }
                            break;
                        case static_cast<int>(Key::LEFT):
                            if (field_coord.z >= inputs.size() || field_coord.z == 0 || field_coord.z == 2) {
                                return;
                            }

                            if (field_coord.x - 1 >= 0) {
                                field_coord.x -= 1;
                            } else {
                                field_coord.x = inputs[field_coord.z][field_coord.y].size() - 1;
                            }
                            break;
                        case static_cast<int>(Key::RIGHT):
                            if (field_coord.z >= inputs.size() || field_coord.z == 0 || field_coord.z == 2) {
                                return;
                            }

                            if (field_coord.x + 1 < inputs[field_coord.z][field_coord.y].size()) {
                                field_coord.x += 1;
                            } else {
                                field_coord.x = 0;
                            }
                            break;
                    }
                    render_page();
                    play_sound("squeak");
                    break;
            }
        }
        else {
            switch (special_key) {
                case static_cast<int>(Key::UP):
                case static_cast<int>(Key::DOWN): {
                    input_header& current_input = inputs[field_coord.z][field_coord.y][field_coord.x];
                    int value = stoi(current_input.field);

                    switch (special_key) {
                        case static_cast<int>(Key::UP):
                            if (value + 1 <= current_input.max_value)
                                value++;
                            else 
                                value = 0;
                            break;
                        case static_cast<int>(Key::DOWN):
                            if (value - 1 >= 0)
                                value--;
                            else 
                                value = current_input.max_value;
                            break;
                    }

                    current_input.field = to_string(value);
                    current_input.local_caret_pos = current_input.field.size();
                    play_sound("select");
                    render_page();
                    break;
                }
            }
        }
    }
}