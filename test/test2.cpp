#include <iostream>
using namespace std;

int get_class_number(double cgpa);

int main() {
    while (true) {
        double cgpa;

        do {
            cout << "Please key-in your cgpa: ";
            cin >> cgpa;
        } while (cgpa < 0 || cgpa > 4);

        cout << "Class: ";
        switch (get_class_number(cgpa)) {
            case 1: cout << "First"; break;
            case 2: cout << "Upper Second"; break;
            case 3: cout << "Lower Second"; break;
            case 4: cout << "Third"; break;
            case 5: cout << "Fail"; break;
        }
        cout << ", " << "CGPA: " << cgpa << endl;
    }
    return 0;
}
int get_class_number(double cgpa) {
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