#define MINIAUDIO_IMPLEMENTATION
#include "globals.h"

int page = static_cast<int>(Page::MAIN);
int previousPage = page;
int selectedOption = static_cast<int>(MainPageOption::FYP_TITLES);
ma_engine sfxEngine;