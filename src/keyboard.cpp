#include <conio.h>
#include "keyboard.h"
#include "globals.h"
#include "renderer.h"
#include "audio.h"
#include "magic_enum.hpp"
using namespace magic_enum;

void detectKeyboardInput() {
    if (_kbhit()) {
        int key = _getch();

        switch (key) {
            case 224: {
                int specialKey = _getch();

                switch (specialKey) {
                    case static_cast<int>(Key::UP): {
                        switch (page) {
                            case static_cast<int>(Page::MAIN): {
                                if (selectedOption - 1 >= 0) {
                                    selectedOption--;
                                }
                                else {
                                    selectedOption = enum_count<MainPageOption>() - 1;
                                }
                                renderPage();
                                playSound("squeak");
                                break;
                            }
                            case static_cast<int>(Page::FYPS): {
                                if (selectedOption - 1 >= 0) {
                                    selectedOption--;
                                } else {
                                    selectedOption = 9;
                                }
                                renderPage();
                                playSound("squeak");
                                break;
                            }
                        }
                        break;
                    }
                    case static_cast<int>(Key::DOWN): {
                        switch (page) {
                            case static_cast<int>(Page::MAIN): {
                                if (selectedOption + 1 < enum_count<MainPageOption>()) {
                                    selectedOption++;
                                }
                                else {
                                    selectedOption = 0;
                                }
                                renderPage();
                                playSound("squeak");
                                break;
                            }
                            case static_cast<int>(Page::FYPS): {
                                if (selectedOption + 1 < 10) {
                                    selectedOption++;
                                } else {
                                    selectedOption = 0;
                                }
                                renderPage();
                                playSound("squeak");
                                break;
                            }
                        }
                        break;
                    }
                }
                break;
            }
            default: {
                switch (key) {
                    case static_cast<int>(Key::ENTER): {
                        playSound("select");
                        keyboardCallback();
                        renderPage();
                        break;
                    }
                    case static_cast<int>(Key::ESCAPE): {
                        page = previousPage;
                        renderPage();
                        playSound("squeak");
                        break;
                    }
                }
                break;
            }
        }
    }
}

void keyboardCallback() {
    switch (page) {
        case static_cast<int>(Page::MAIN): {
            switch (selectedOption) {
                case static_cast<int>(MainPageOption::EXIT): {
                    exit(0);
                    break;
                }
                case static_cast<int>(MainPageOption::ACCOUNT): {
                    page = static_cast<int>(Page::SIGN_UP);
                    break;
                }   
                case static_cast<int>(MainPageOption::CONSOLE): {
                    page = static_cast<int>(Page::CONSOLE);
                    break;
                }
                case static_cast<int>(MainPageOption::FYP_TITLES): {
                    page = static_cast<int>(Page::FYPS);
                    break;
                } 
            }
            break;
        }
        case static_cast<int>(Page::FYPS): {
            // Implement key callback/events to fyp titles page.
        }
    }
}