#pragma once
#include "SDL_render.h"
#include "SDL_video.h"

#define LOG() SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Fix:- %s", SDL_GetError())
#define LOGARGS(x) SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Fix:- %s", x)

void cpRun();
void cpQuit();
int cpGetW();
int cpGetH();
int cpGetX();
int cpGetY();
const char* cpGetTitle();
SDL_Window*& cpGetWindow();
SDL_Renderer*& cpGetRenderer();