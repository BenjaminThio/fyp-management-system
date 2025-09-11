#include <iostream>
#include <string>
using namespace std;

string get_class(double cgpa);
int get_class_number(double cgpa);

int main() {
    while (true) {
        double cgpa;

        do {
            cout << "Please key-in your cgpa: ";
            cin >> cgpa;
        } while (cgpa < 0 || cgpa > 4);

        cout << "Class: " << get_class(cgpa) << ", " << "CGPA: " << cgpa << endl;
    }
    return 0;
}
string get_class(double cgpa) {
    switch (get_class_number(cgpa)) {
        case 1: return "First";
        case 2: return "Upper Second";
        case 3: return "Lower Second";
        case 4: return "Third";
        case 5: return "Fail";
        default: throw runtime_error("Not a valid cgpa.");
    }
}
int get_class_number(double cgpa) {
    if (cgpa < 0 || cgpa > 4) throw runtime_error("Not a valid cpga."); 

    int class_counter = 0;

    class_counter++;
    if (cgpa >= 3.5 && cgpa <= 4) return class_counter; // one-way selection
    class_counter++;
    if (cgpa >= 3.00 && cgpa <= 3.49) return class_counter;
    class_counter++;
    if (cgpa >= 2.20 && cgpa <= 2.99) return class_counter;
    class_counter++;
    if (cgpa >= 2.00 && cgpa <= 2.19) return class_counter;
    class_counter++;

    return class_counter;
}