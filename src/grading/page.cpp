#include <sstream>
#include <conio.h>
#include "keyboard.h"
#include "globals.h"
#include "renderer.h"
#include "audio.h"
#include "table.h"
using namespace std;

namespace grading {
    void push_frame(ostringstream& renderer) {
        renderer << "TEST" << endl;
    }

    void keyboard_input_callback() {
        if (_kbhit()) {
            int key = _getch();

            switch (key) {
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