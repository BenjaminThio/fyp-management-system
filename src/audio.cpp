#include "audio.h"
#include "globals.h"

void playSound(string name) {
    ma_engine_play_sound(&sfxEngine, ("../sound/snd_" + name + ".wav").c_str(), NULL);
}