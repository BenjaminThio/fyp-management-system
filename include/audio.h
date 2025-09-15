#ifndef AUDIO_H
#define AUDIO_H

#include "miniaudio.h"
#include <string>
using namespace std;

extern ma_engine sfx_engine;

void play_sound(string);

#endif