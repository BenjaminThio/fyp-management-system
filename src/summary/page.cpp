#include <sstream>
#include <conio.h>
#include <map>
#include <functional>
#include "keyboard.h"
#include "globals.h"
#include "renderer.h"
#include "table.h"
using namespace std;

namespace summary {
    string id;
    map<double, int, greater<double>> summary_map = {
        { 4.0, 0 },
        { 3.67, 0 },
        { 3.33, 0 },
        { 3.0, 0 },
        { 2.67, 0 },
        { 2.33, 0 },
        { 2.0, 0 },
        { 0.0, 0 }
    };

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

    static string get_class(double cgpa) {
        if (cgpa < 0 || cgpa > 4) throw runtime_error("Not a valid cpga.");

        if (cgpa >= 3.5 && cgpa <= 4) return "First";
        else if (cgpa >= 3.00 && cgpa <= 3.49) return "Upper Second";
        else if (cgpa >= 2.20 && cgpa <= 2.99) return "Lower Second";
        else if (cgpa >= 2.00 && cgpa <= 2.19) return "Third";
        else return "Fail";
    }

    static string to_fixed(double value, int precision = 2) {
        ostringstream oss;
        oss << std::fixed << setprecision(precision) << value;
        
        return oss.str();
    }

    static void reset_data() {
        id = "";
        summary_map = {
            { 4.0, 0 },
            { 3.33, 0 },
            { 3.0, 0 },
            { 2.67, 0 },
            { 2.33, 0 },
            { 2.0, 0 },
            { 0.0, 0 }
        };
    }

    static bool is_grading_completed(json& approved_user) {
        if (!approved_user.contains("grading")) {
            return false;
        }

        json val = approved_user["grading"];

        for (size_t i = 1; i <= 2; i++) {
            if (val.contains(json("fyp" + to_string(i)))) {
                if (!val["fyp" + to_string(i)].contains(json("supervisor")) || !val["fyp" + to_string(i)].contains(json("moderator"))) {
                    return false;
                }
            } else {
                return false;
            }
        }
        return true;
    }

    void summary(const string& fyp_id) {
        id = fyp_id;
        json users = get_users();

        for (auto& approved_email : fyps[id]["wishlistApproved"].as_list()) {
            json approved_user = users[approved_email.as_string()];

            if (!is_grading_completed(approved_user)) continue;

            int64_t sp_fyp_1_report = approved_user["grading"]["fyp1"]["supervisor"]["fyp_1_report"].as_int();
            int64_t sp_fyp_1_pre = approved_user["grading"]["fyp1"]["supervisor"]["fyp_1_presentation"].as_int();
            int64_t sp_fyp_1_effort = approved_user["grading"]["fyp1"]["supervisor"]["fyp_1_effort"].as_int();

            int64_t mod_fyp_1_report = approved_user["grading"]["fyp1"]["moderator"]["fyp_1_report"].as_int();
            int64_t mod_fyp_1_pre = approved_user["grading"]["fyp1"]["moderator"]["fyp_1_presentation"].as_int();

            int64_t sp_final_report = approved_user["grading"]["fyp2"]["supervisor"]["final_report"].as_int();
            int64_t sp_product_demo = approved_user["grading"]["fyp2"]["supervisor"]["product_demo"].as_int();
            int64_t sp_fyp_2_pre = approved_user["grading"]["fyp2"]["supervisor"]["fyp_2_presentation"].as_int();
            int64_t sp_fyp_2_effort =  approved_user["grading"]["fyp2"]["supervisor"]["fyp_2_effort"].as_int();

            int64_t mod_final_report = approved_user["grading"]["fyp2"]["moderator"]["final_report"].as_int();
            int64_t mod_product_demo = approved_user["grading"]["fyp2"]["moderator"]["product_demo"].as_int();
            int64_t mod_fyp_2_pre = approved_user["grading"]["fyp2"]["moderator"]["fyp_2_presentation"].as_int();

            double total_marks = ((sp_fyp_1_report + mod_fyp_1_report) / 2.0) + ((sp_fyp_1_pre + mod_fyp_1_pre) / 2.0) + (sp_fyp_1_effort / 1.0)
                                + ((sp_final_report + mod_final_report) / 2.0) + ((sp_product_demo + mod_product_demo) / 2.0) + ((sp_fyp_2_pre + mod_fyp_2_pre) / 2.0)
                                + (sp_fyp_2_effort / 1.0);
            double cgpa = marks_to_cgpa(total_marks);

            summary_map[cgpa] += 1;
        }
        redirect(static_cast<int>(Page::SUMMARY));
    }

    void push_frame(ostringstream& renderer) {
        vector<vector<string>> table = {
            { "CGPA", "Class", "Number of Students"  }
        };

        for (const auto& [key, val] : summary_map) {
            table.push_back({ { to_fixed(key), get_class(key), to_string(val) } });
        }

        renderer
        << " __   _     _      _       __    ___   _    \n( (` | | | | |\\/| | |\\/|  / /\\  | |_) \\ \\_/ \n_)_) \\_\\_/ |_|  | |_|  | /_/--\\ |_| \\  |_|  " << endl << endl
        << generate_table(table);
    }

    void keyboard_input_callback() {
        if (_kbhit()) {
            int key = _getch();

            switch (key) {
                case 0:
                case 224: {
                    // int special_key = _getch();
                    break;
                }
                case static_cast<int>(Key::ESCAPE): {
                    reset_data();
                    return_page();
                    break;
                }
            }
        }
    }
}