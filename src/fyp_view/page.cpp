#include <sstream>
#include <conio.h>
#include "keyboard.h"
#include "globals.h"
#include "renderer.h"
#include "audio.h"
using namespace std;

namespace fyp_view {
    void push_frame(ostringstream& renderer) {
        renderer << "Title: Why Are You Gay?" << endl;
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
                    page = previous_page;

                    render_page();
                    play_sound("squeak");
                    break;
                }
            }
        }
    }
}