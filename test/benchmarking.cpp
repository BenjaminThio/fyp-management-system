#include <iostream>
#include "Instrumentor.h"
using namespace std;

void range_based_switch_case() {
    InstrumentationTimer timer("range_based_switch_case");

    for (size_t i = 0; i < 100000; i++) {
        switch (i) {
            case 0 ... 5000:
                //cout << "IN";
                break;
            case 5001 ... 10000:
                //cout << "OUT";
                break;
        }
    }
}

void range_based_if_else() {
    InstrumentationTimer timer("range_based_if_else");

    for (size_t i = 0; i < 100000; i++) {
        if (i >= 0 && i <= 5000) {
            //cout << "IN";
        }
        else if (i >= 5001 && i <= 10000) {
            //cout << "OUT";
        }
    }
}

int main() {
    Instrumentor::Get().BeginSession("Range Check Benchmark");

    {
        range_based_switch_case();
    }

    {
        range_based_if_else();
    }

    Instrumentor::Get().EndSession();
    return 0;
}