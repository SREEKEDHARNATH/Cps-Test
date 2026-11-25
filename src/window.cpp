#include "window.hpp"
#include "SDL.h"
#include "SDL_events.h"
#include "SDL_image.h"
#include "SDL_keycode.h"
#include "SDL_render.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include "api.hpp"
#include "listeners.hpp"

bool init();
void handleinputs(bool& running);
void cpRun(){
    if (!init()){
        LOG();
        return;
    }
    bool running = true;
    float fps=240;

    cpImage img(255,0,0);
    SDL_ShowWindow(cpGetWindow());
    while (running) {
        handleinputs(running);
        SDL_RenderClear(cpGetRenderer());
        img.draw();
        SDL_RenderPresent(cpGetRenderer());
        
        cpKUpdate();
        cpMUpdate();
        SDL_Delay(fps/1000);
    }
}
void handleinputs(bool& running){
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                running=false;
                break;
            case SDL_KEYUP:
            case SDL_KEYDOWN:
                cpKReg(e.key);
                break;
            case SDL_MOUSEMOTION:
                cpMReg(e.motion);
                break;
            case SDL_MOUSEWHEEL:
                cpMReg(e.wheel);
                break;
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEBUTTONDOWN:
                cpMReg(e.button);
                break;
        }
    }
    if (cpKIsPresed(SDLK_ESCAPE)){
        running=false;
    }
}
bool init(){
    int img_flags=IMG_INIT_JPG | IMG_INIT_PNG;
    if (SDL_Init(SDL_INIT_EVERYTHING)){
        return false;
    }
    if ((IMG_Init(img_flags) & img_flags) != img_flags){
        return false;
    }
    if (!(cpGetWindow() = SDL_CreateWindow(cpGetTitle(), SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,cpGetW(),cpGetH(),SDL_WINDOW_HIDDEN))){
        return false;
    }
    if (!(cpGetRenderer() = SDL_CreateRenderer(cpGetWindow(),-1,0))){
        return false;
    }
    return true;
}
void cpQuit(){
    if (cpGetRenderer()){
        SDL_DestroyRenderer(cpGetRenderer());
    }
    if (cpGetWindow()){
        SDL_DestroyWindow(cpGetWindow());
    }
    IMG_Quit();
    SDL_Quit();
}
int cpGetW(){
    return 1280;
}
int cpGetH(){
    return 720;
}
const char* cpGetTitle(){
    return "Cps Test";
}
SDL_Window*& cpGetWindow(){
    static SDL_Window* obj = NULL;
    return obj;
}
SDL_Renderer*& cpGetRenderer(){
    static SDL_Renderer* obj = NULL;
    return obj;
}