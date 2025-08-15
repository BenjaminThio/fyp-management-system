#define MINIAUDIO_IMPLEMENTATION
#include "audio.h"

ma_engine sfx_engine;

void play_sound(string name) {
    ma_engine_play_sound(&sfx_engine, ("../sound/snd_" + name + ".wav").c_str(), NULL);
}