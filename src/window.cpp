#include <format>
#include "window.hpp"
#include "SDL.h"
#include "SDL_events.h"
#include "SDL_image.h"
#include "SDL_keycode.h"
#include "SDL_mixer.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"
#include "SDL_ttf.h"
#include "SDL_video.h"
#include "api.hpp"
#include "listeners.hpp"
#include "utils.hpp"
#include <string>

bool init();
void handleinputs(bool& running);

void cpRun(){
    if (!init()){
        LOG();
        return;
    }
    cpFont f("assets/font.ttf");
    cpText t(f,"0");
    cpText cps(f,"0");
    cpText cpst(f,"Cps");
    cpText tt(f,"Clicks");
    tt.i.r.x=60;
    cps.i.r.x=200;
    cpst.i.r.x=250;
    button bl(f,"Left Click",100,100);
    button bm(f,"Middle Click",500,100);
    button br(f,"Right Click",900,100);
    bl.val=1;
    bm.val=2;
    br.val=3;

    int clicks=0;
    bool running = true;
    float fps=240;
    bool start=false;
    int b=0;
    Uint64 ticks = SDL_GetTicks64();
    int sec=5;
    SDL_ShowWindow(cpGetWindow());
    while (running) {
        handleinputs(running);
        SDL_RenderClear(cpGetRenderer());
        if (start){
            clicks+=(int)cpMIsPressed(b);
            t.set(f, std::to_string(clicks).c_str());
            cps.set(f, std::format("{}", (clicks/(float)sec)).c_str());
            if ((ticks + sec*1000) < (SDL_GetTicks64()-(fps/1000))){
                start=false;
                clicks=0;
                ticks=SDL_GetTicks64();
            }
            t.i.draw();
            cps.i.draw();
            cpst.i.draw();
            tt.i.draw();
        }
        else {
            bl.draw();
            bm.draw();
            br.draw();
        }
        SDL_RenderPresent(cpGetRenderer());
        if (!start && ((ticks + 3000) < SDL_GetTicks64())){
            if (bl.haspressed()){
                start=true;
                b=bl.val;
                ticks = SDL_GetTicks64();
            }
            else if (bm.haspressed()){
                start=true;
                b=bm.val;
                ticks = SDL_GetTicks64();
            }
            else if (br.haspressed()){
                start=true;
                b=br.val;
                ticks = SDL_GetTicks64();
            }
        }
        
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