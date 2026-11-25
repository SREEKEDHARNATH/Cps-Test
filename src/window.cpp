#include "window.hpp"
#include "SDL.h"
#include "SDL_error.h"
#include "SDL_events.h"
#include "SDL_render.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include <utility>

std::pair<bool, const char*> init();
void handleinputs(bool& running);
void cpRun(){
    auto initres = init();
    if (!initres.first){
        LOG("Fix:- %s", initres.second);
        return;
    }
    bool running = true;
    float fps=240;

    SDL_ShowWindow(cpGetWindow());
    while (running) {
        handleinputs(running);

        SDL_RenderClear(cpGetRenderer());
        SDL_RenderPresent(cpGetRenderer());
        
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
        }
    }
}
std::pair<bool, const char*> init(){
    if (SDL_Init(SDL_INIT_EVERYTHING)){
        return {false,SDL_GetError()};
    }
    if (!(cpGetWindow() = SDL_CreateWindow(cpGetTitle(), SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,cpGetW(),cpGetH(),SDL_WINDOW_HIDDEN))){
        return {false,SDL_GetError()};
    }
    if (!(cpGetRenderer() = SDL_CreateRenderer(cpGetWindow(),-1,0))){
        return {false,SDL_GetError()};
    }
    return {true,""};
}
void cpQuit(){
    if (cpGetRenderer()){
        SDL_DestroyRenderer(cpGetRenderer());
    }
    if (cpGetWindow()){
        SDL_DestroyWindow(cpGetWindow());
    }
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