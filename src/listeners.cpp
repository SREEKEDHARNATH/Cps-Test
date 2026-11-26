#include "listeners.hpp"
#include "SDL_events.h"
#include "SDL_keyboard.h"
#include "SDL_keycode.h"
#include "SDL_mouse.h"
#include "SDL_scancode.h"
#include "window.hpp"

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

bool buttons[3]={false},prvbuttons[3]={false};
int x=0,y=0;
float scrlx=0,scrly=0;

void cpMReg(SDL_MouseMotionEvent m){
    x=m.x;
    y=m.y;
}
void cpMReg(SDL_MouseButtonEvent b){
    auto bu = b.button;
    if (bu >0 && bu < 4){
        buttons[bu-1] = (b.state==SDL_PRESSED);
    }
}
void cpMReg(SDL_MouseWheelEvent w){
    scrlx=w.preciseX;
    scrly=w.preciseY;

    if (w.direction == SDL_MOUSEWHEEL_FLIPPED){
        scrlx*=-1;
        scrlx*=-1;
    }
}
bool cpMIsPressed(int b){
    if (b>0 && b<4){
        return buttons[b-1] && !prvbuttons[b-1];
    }
    return false;
}
bool cpMIsDown(int b){
    if (b>0 && b<4){
        return buttons[b-1];
    }
    return false;
}
int cpMGetX(){
    return x;
}
int cpMGetY(){
    return y;
}
float cpMGetScrlx(){
    return scrlx;
}
float cpMGetScrly(){
    return scrly;
}
void cpMUpdate(){
    for (int i=0;i<3;i++){
        prvbuttons[i]=buttons[i];
    }
    scrlx=0;
    scrly=0;
}