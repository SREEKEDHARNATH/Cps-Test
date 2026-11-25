#pragma once

#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_scancode.h"
void cpKReg(SDL_KeyboardEvent k);
bool cpKIsPresed(SDL_KeyCode k);
bool cpKIsPresed(SDL_Scancode sc);
bool cpKIsDown(SDL_KeyCode k);
bool cpKIsDown(SDL_Scancode sc);
void cpKUpdate();

void cpMReg(SDL_MouseMotionEvent m);
void cpMReg(SDL_MouseButtonEvent b);
void cpMReg(SDL_MouseWheelEvent w);
bool cpMIsPressed(int b);
bool cpMIsDown(int b);
int cpMGetX();
int cpMGetY();
float cpMGetScrlx();
float cpMGetScrly();
void cpMUpdate();