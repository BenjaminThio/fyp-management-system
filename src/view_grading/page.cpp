#include <sstream>
#include <string>
#include <conio.h>
#include "keyboard.h"
#include "globals.h"
#include "renderer.h"
#include "table.h"
using namespace std;

namespace view_grading {
    static array<vector<vector<string>>, 2> tables;

    bool is_grading_completed() {
        if (!user.contains("grading")) {
            return false;
        }

        json val = user["grading"];

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

    string to_fixed(double value, int precision = 2) {
        ostringstream oss;
        oss << std::fixed << setprecision(precision) << value;
        
        return oss.str();
    }

    static string get_class(double cgpa) {
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

    void push_frame(ostringstream& renderer) {
        tables = {
            vector<vector<string>>{
                { "Component", "Supervisor", "Moderator", "Average" },
                { "FYP 1 Report (15%)", "", "", "" },
                { "FYP 1 Presentation (10%)", "", "", "" },
                { "FYP 1 Effort (5%)", "", "", "" }
            },
            vector<vector<string>>{
                { "Component", "Supervisor", "Moderator", "Average" },
                { "Final Report (20%)", "", "", "" },
                { "Product Demo (35%)", "", "", "" },
                { "FYP 2 Presentation (10%)", "", "", "" },
                { "FYP 2 Effort (5%)", "", "", "" }
            }
        };

        if (!has_approved_fyp()) {
            renderer << "You don’t have any approved FYP." << endl;
            return;
        }
        
        if (!is_grading_completed()) {
            renderer << "Grading is in progress." << endl;
            return;
        }

        /**/
        string approved_fyp = get_user_approved_fyps()[0]["id"].as_string();
        int64_t sp_fyp_1_report = user["grading"]["fyp1"]["supervisor"]["fyp_1_report"].as_int();
        int64_t sp_fyp_1_pre = user["grading"]["fyp1"]["supervisor"]["fyp_1_presentation"].as_int();
        int64_t sp_fyp_1_effort = user["grading"]["fyp1"]["supervisor"]["fyp_1_effort"].as_int();

        tables[0][1][1] = to_string(sp_fyp_1_report) + "%";
        tables[0][2][1] = to_string(sp_fyp_1_pre) + "%";
        tables[0][3][1] = to_string(sp_fyp_1_effort) + "%";

        int64_t mod_fyp_1_report = user["grading"]["fyp1"]["moderator"]["fyp_1_report"].as_int();
        int64_t mod_fyp_1_pre = user["grading"]["fyp1"]["moderator"]["fyp_1_presentation"].as_int();

        tables[0][1][2] = to_string(mod_fyp_1_report) + "%";
        tables[0][2][2] = to_string(mod_fyp_1_pre) + "%";

        tables[0][1][3] = to_fixed((sp_fyp_1_report + mod_fyp_1_report) / 2.0) + "%";
        tables[0][2][3] = to_fixed((sp_fyp_1_pre + mod_fyp_1_pre) / 2.0) + "%";
        tables[0][3][3] = to_fixed(sp_fyp_1_effort / 1.0) + "%";

        int64_t sp_final_report = user["grading"]["fyp2"]["supervisor"]["final_report"].as_int();
        int64_t sp_product_demo = user["grading"]["fyp2"]["supervisor"]["product_demo"].as_int();
        int64_t sp_fyp_2_pre = user["grading"]["fyp2"]["supervisor"]["fyp_2_presentation"].as_int();
        int64_t sp_fyp_2_effort =  user["grading"]["fyp2"]["supervisor"]["fyp_2_effort"].as_int();

        tables[1][1][1] = to_string(sp_final_report) + "%";
        tables[1][2][1] = to_string(sp_product_demo) + "%";
        tables[1][3][1] = to_string(sp_fyp_2_pre) + "%";
        tables[1][4][1] = to_string(sp_fyp_2_effort) + "%";

        int64_t mod_final_report = user["grading"]["fyp2"]["moderator"]["final_report"].as_int();
        int64_t mod_product_demo = user["grading"]["fyp2"]["moderator"]["product_demo"].as_int();
        int64_t mod_fyp_2_pre = user["grading"]["fyp2"]["moderator"]["fyp_2_presentation"].as_int();

        tables[1][1][2] = to_string(mod_final_report) + "%";
        tables[1][2][2] = to_string(mod_product_demo) + "%";
        tables[1][3][2] = to_string(mod_fyp_2_pre) + "%";

        tables[1][1][3] = to_fixed((sp_final_report + mod_final_report) / 2.0) + "%";
        tables[1][2][3] = to_fixed((sp_product_demo + mod_product_demo) / 2.0) + "%";
        tables[1][3][3] = to_fixed((sp_fyp_2_pre + mod_fyp_2_pre) / 2.0) + "%";
        tables[1][4][3] = to_fixed(sp_fyp_2_effort / 1.0) + "%";

        double total_marks = ((sp_fyp_1_report + mod_fyp_1_report) / 2.0) + ((sp_fyp_1_pre + mod_fyp_1_pre) / 2.0) + (sp_fyp_1_effort / 1.0)
                            + ((sp_final_report + mod_final_report) / 2.0) + ((sp_product_demo + mod_product_demo) / 2.0) + ((sp_fyp_2_pre + mod_fyp_2_pre) / 2.0)
                            + (sp_fyp_2_effort / 1.0);
        double cgpa = marks_to_cgpa(total_marks);


        renderer << endl
        << "Title: " << fyps[approved_fyp]["info"]["name"].as_string() << endl
        << "ID: " << approved_fyp << endl << endl
        << "FYP 1" << endl
        << generate_table(tables[0]) << endl
        << "FYP 2" << endl
        << generate_table(tables[1]) << endl
        << "Final Marks: " << total_marks << '%' << endl
        << "CGPA: " << fixed << setprecision(2) << cgpa << endl
        << "Class: " << get_class(cgpa) << endl;
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
                    return_page();
                    break;
                }
            }
        }
    }
}