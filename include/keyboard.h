#ifndef KEYBOARD_H
#define KEYBOARD_H

enum class Key: int {
    UP = 72,
    LEFT = 75,
    DOWN = 80,
    RIGHT = 77,
    ENTER = 13,
    ESCAPE = 27,
    BACKSPACE = 8,
    TAB = 9
};

void detect_keyboard_input(void);

#endif