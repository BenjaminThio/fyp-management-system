#include <iostream>
#include "globals.h"
#include "renderer.h"
#include "keyboard.h"
using namespace std;

int main() {
    if (ma_engine_init(NULL, &sfxEngine) != MA_SUCCESS) {
        cerr << "Failed to initialise SFX engine" << endl;
        getchar();
        return -1;
    }

    renderPage();

    while (true) detectKeyboardInput();
}