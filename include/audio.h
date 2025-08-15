#ifndef AUDIO_H
#define AUDIO_H

#include <string>
#include "miniaudio.h"
using namespace std;

extern ma_engine sfx_engine;

void play_sound(string);

#endif