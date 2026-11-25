#include "listeners.hpp"
#include "SDL_events.h"
#include "SDL_keyboard.h"
#include "SDL_keycode.h"
#include "SDL_scancode.h"
bool keys[SDL_NUM_SCANCODES]={false};
bool prvkeys[SDL_NUM_SCANCODES]={false};

void cpKReg(SDL_KeyboardEvent k){
    auto sc = k.keysym.scancode;
    if (sc > 0 && sc < SDL_NUM_SCANCODES){
        keys[sc]=(k.state==SDL_PRESSED);
    }
}
bool cpKIsPresed(SDL_KeyCode k){
    return cpKIsPresed(SDL_GetScancodeFromKey(k));
}
bool cpKIsPresed(SDL_Scancode sc){
    if (sc > 0 && sc < SDL_NUM_SCANCODES){
        return keys[sc] && !prvkeys[sc];
    }
    return false;
}
bool cpKIsDown(SDL_KeyCode k){
    return cpKIsDown(SDL_GetScancodeFromKey(k));
}
bool cpKIsDown(SDL_Scancode sc){
    if (sc > 0 && sc < SDL_NUM_SCANCODES){
        return keys[sc];
    }
    return false;
}
void cpKUpdate(){
    for (int i=1;i<SDL_NUM_SCANCODES;i++){
        prvkeys[i]=keys[i];
    }
}