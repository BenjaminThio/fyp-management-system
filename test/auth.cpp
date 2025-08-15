#include <iostream>
#include <thread>
#include <conio.h>
#include <atomic>

std::atomic<bool> running(true);

void thread_1() {
    int a;
    std::cout << "Test: ";
    std::cin >> a;
    running = false;    // stop the other thread when cin is done
}

void thread_2() {
    while (running) {
        if (_kbhit()) {
            int key = _getch();
            if (key == 27) {  // ESC
                std::cout << "Testing123\n";
                running = false;   // stop both threads
            }
        }
    }
}

int main() {
    std::thread t1(thread_1);
    std::thread t2(thread_2);

    t1.join();
    t2.join();
    return 0;
}
/*
#include <iostream>
#include <thread>
#include <conio.h>
#include <keyboard.h>
using namespace std;

void thread_1() {
    int a;

    cout << "Test: ";
    cin >> a;
}

void thread_2() {
    while (_kbhit()) {
        int key = _getch();

        switch (key) {
            case 27: {
                cout << "Testing123" << endl;
                break;
            }
        }
    }
}

int main() {
    thread t1(thread_1);
    thread t2(thread_2);

    t1.join();
    t2.join();

    return 0;
}
    */