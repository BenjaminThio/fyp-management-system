#include <iostream>
#include <string>
#include <sstream>
#include <conio.h>
#include "keyboard.h"
#include "globals.h"
#include "input.h"
#include "renderer.h"
#include "audio.h"
using namespace std;

namespace sign_up {
    void push_frame(ostringstream& renderer) {
        string userInput;

        renderer << "Username:\n\nPassword:\n\nEmail:";
        cout << renderer.str();
        set_input_cursor_position(0, 7);
        getline(cin, userInput);
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