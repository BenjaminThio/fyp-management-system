#include <iostream>
using namespace std;

double celcius_to_fahrenheit(double celcius) {
    return (celcius * 9/5) + 32;
}

double fahrenheit_to_celsius(double fahrenheit) {
    return (fahrenheit - 32) * 5/9;
}

int main() {
    cout << celcius_to_fahrenheit(0) << endl;
    cout << fahrenheit_to_celsius(50) << endl;
}