#include "window.hpp"
#include "SDL.h"
#include "SDL_events.h"
#include "SDL_image.h"
#include "SDL_keycode.h"
#include "SDL_mixer.h"
#include "SDL_render.h"
#include "SDL_timer.h"
#include "SDL_ttf.h"
#include "SDL_video.h"
#include "api.hpp"
#include "listeners.hpp"
#include "utils.hpp"
#include <iostream>

bool init();
void handleinputs(bool& running);
void cpRun(){
    if (!init()){
        LOG();
        return;
    }

    cpFont f("assets/font.ttf");
    button b(f,"Hello World",100,100,[](){
        std::cout << "hello\n";
        return;
    });
    bool running = true;
    float fps=240;

    cpImage img(255,0,0);
    SDL_ShowWindow(cpGetWindow());
    while (running) {
        handleinputs(running);
        SDL_RenderClear(cpGetRenderer());
        img.draw();
        b.draw();
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
    int sound_flags=MIX_INIT_MP3;
    if (SDL_Init(SDL_INIT_EVERYTHING)){
        return false;
    }
    if ((IMG_Init(img_flags) & img_flags) != img_flags){
        return false;
    }
    if (TTF_Init()){
        return false;
    }
    if ((Mix_Init(sound_flags) & sound_flags) != sound_flags){
        return false;
    }
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024)){
        return false;
    }
    if (!(cpGetWindow() = SDL_CreateWindow(cpGetTitle(), cpGetX(),cpGetY(),cpGetW(),cpGetH(),SDL_WINDOW_HIDDEN))){
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
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
int cpGetW(){
    return 1280;
}
int cpGetH(){
    return 720;
}
int cpGetX(){
    return SDL_WINDOWPOS_CENTERED;
}
int cpGetY(){
    return SDL_WINDOWPOS_CENTERED;
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