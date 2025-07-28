#ifndef KEYBOARD_H
#define KEYBOARD_H

enum class Key: int {
    UP = 72,
    LEFT = 75,
    DOWN = 80,
    RIGHT = 77,
    ENTER = 13,
    ESCAPE = 27
};

void detectKeyboardInput(void);
void keyboardCallback(void);

#endif