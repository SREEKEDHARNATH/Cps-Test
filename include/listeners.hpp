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