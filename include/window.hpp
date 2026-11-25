#pragma once
#include "SDL_render.h"
#include "SDL_video.h"
#define LOG(x,y) SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, x,y)
void cpRun();
void cpQuit();
int cpGetW();
int cpGetH();
const char* cpGetTitle();
SDL_Window*& cpGetWindow();
SDL_Renderer*& cpGetRenderer();