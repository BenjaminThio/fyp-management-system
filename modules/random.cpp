#include <iostream>
#include <random>
using namespace std;

int randi_range(int, int);
double randf_range(double, double);
double randf(void);

int main() {
    for (int i = 0; i < 100; i++) {
        cout << randf() << endl;
    }
    return 0;
}

int randi_range(int start, int end) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution distr(start, end);

    return distr(gen);
}

double randf_range(double start, double end) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distr(start, end);

    return distr(gen);
}

double randf() { return randf_range(0.0, 1.0); }

/*
Using srand(time(NULL)) inside a fast loop is a bad idea.
time(NULL) only changes once per second, but the loop runs much faster (often in milliseconds).
This means you're seeding with the same value repeatedly, so you’ll get the same or very similar random numbers every time.
Unless the loop takes more than a second to run.

#include <cstdlib>
#include <ctime>

int randi_range(int start, int end) {
    srand(time(NULL));

    return start + (rand() % (end - start + 1));
}
*/
