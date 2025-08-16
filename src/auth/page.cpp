#define WIN32_LEAN_AND_MEAN
#define NOMIMAX
#include <windows.h>

#include <iostream>
#include <string>
#include <sstream>
#include <conio.h>
#include "keyboard.h"
#include "globals.h"
#include "input.h"
#include "renderer.h"
#include "audio.h"
#include <array>
#include <auth/page.h>
using namespace std;

namespace sign_up {
    static int field_index = 0;
    static array<string, 3> fields;
    static array<int, 3> cursor_input_pos = { 0, 0, 0 };

    void push_frame(ostringstream& renderer, array<int, 2>& manual_cursor_input_pos) {
        string userInput;

        renderer
        << "Username:"
        << endl
        << fields[0]
        << endl
        << "Password:"
        << endl
        << fields[1]
        << endl
        << "Email:"
        << endl
        << fields[2];
        manual_cursor_input_pos = { cursor_input_pos[field_index], (field_index * 2) + 7 };
    }

    void keyboard_input_callback() {
        array<int, 2> keyboard_input = better_cin(fields[field_index], cursor_input_pos[field_index], true);
        int key = keyboard_input[0];
        //int special_key = keyboard_input[1];
        
        switch (key) {
            case static_cast<int>(Key::ESCAPE):
                page = previous_page;

                render_page();
                play_sound("squeak");
                break;
            case static_cast<int>(Key::TAB):
                if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
                    if (field_index - 1 >= 0) field_index--;
                    else field_index = fields.size() - 1;
                } else {
                    if (field_index + 1 < fields.size()) field_index++;
                    else field_index = 0;
                }

                render_page();
                play_sound("squeak");
                break;
        }
    }
}