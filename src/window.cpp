#include <cstdlib>
#include <format>
#include "window.hpp"
#include "SDL.h"
#include "SDL_events.h"
#include "SDL_image.h"
#include "SDL_keycode.h"
#include "SDL_mixer.h"
#include "SDL_rect.h"
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

struct cpsSystem{
    int sec=5;
    int b=0;
    SDL_KeyCode k=(SDL_KeyCode)0;
    Uint64 ticks;
    cpImage clickarea;
    cpText t;
    cpText cps;
    cpText cpst;
    cpText tt;
    int clicks=0;
    int fps=240;
    float cpsscore=0;
    float peakscore=0;

    cpsSystem(cpFont& f) : clickarea(100,100,100),t(f,"0"), cps(f,"0"), cpst(f,"Cps"), tt(f,"Clicks"){
        clickarea.r.x=cpGetW()/5.0f;
        clickarea.r.y=cpGetH()/5.0f;
        clickarea.r.w=cpGetW()/1.5f;
        clickarea.r.h=cpGetH()/1.5f;
        tt.i.r.x=60;
        cps.i.r.x=200;
        cpst.i.r.x=250;
    }
    bool update(cpFont& f){
        if (b == 0 && k != 0){
            clicks+=(int)cpKIsPressed(k);
        }
        else if (k==0 && b != 0){
            SDL_Rect mouse = {cpMGetX(),cpMGetY(),1,1};
            if (SDL_HasIntersection(&clickarea.r, &mouse)){
                clicks+=(int)cpMIsPressed(b);
            }
        }
        t.set(f, std::to_string(clicks).c_str());
        auto st=std::format("{}", (clicks/(float)(SDL_GetTicks64()-ticks)/1000));
        auto dt=st.find(".");
        if (dt != st.npos){
            st=st.substr(0,dt+2);
        }
        cps.set(f, st.c_str());

        auto cp=std::stof(st);
        peakscore=(cp>peakscore ? cp : peakscore);
        if ((ticks + sec*1000) < (SDL_GetTicks64()-(fps/1000))){
            cpsscore=cp;
            clicks=0;
            ticks=SDL_GetTicks64();
            return false;
        }
        return true;
    }
    void draw(){
        t.i.draw();
        cps.i.draw();
        cpst.i.draw();
        tt.i.draw();
        if (k==0 && b != 0){
            clickarea.draw();
        }
    }
    void reset(){
        k=(SDL_KeyCode)0;
        b=0;
    }
};
void cpRun(){
    if (!init()){
        LOG();
        return;
    }

    cpFont f("assets/font.ttf");
    button buttons[12];
    buttons[0].set(f,"Left Click",100,100,1);
    buttons[1].set(f,"Middle Click",500,100,2);
    buttons[2].set(f,"Right Click",900,100,3);

    buttons[3].set(f,"Space",150,300,1);
    {
        int x=500;
        int y=325;
        int gap=80;
        buttons[4].set(f,"W",x,y,SDLK_w,true);
        buttons[5].set(f,"A",x-gap,y+gap,SDLK_a,true);
        buttons[6].set(f,"S",x,y+(2*gap),SDLK_s,true);
        buttons[7].set(f,"D",x+gap,y+gap,SDLK_d,true);
    }
    {
        int x=800;
        int y=325;
        int gap=80;
        buttons[8].set(f,"^",x,y,SDLK_UP,true);
        buttons[9].set(f,"<",x-gap,y+gap,SDLK_LEFT,true);
        buttons[10].set(f,"v",x,y+(2*gap),SDLK_DOWN,true);
        buttons[11].set(f,">",x+gap,y+gap,SDLK_RIGHT,true);
    }
    
    cpsSystem sys(f);

    bool running = true;
    float fps=240;
    bool start=false;
    sys.ticks=0;
    SDL_ShowWindow(cpGetWindow());
    while (running) {
        handleinputs(running);
        SDL_RenderClear(cpGetRenderer());
        if (start){
            start=sys.update(f);
            sys.draw();
            if (!start){
                sys.reset();
            }
        }
        else {
            for (auto& bu : buttons){
                bu.draw();
            }
        }
        SDL_RenderPresent(cpGetRenderer());
        if (!start && ((sys.ticks + 3000) < SDL_GetTicks64())){
            for (auto& bu : buttons){
                if (bu.haspressed()){
                    start=true;
                    if (bu.val > 3){
                        sys.k=(SDL_KeyCode)bu.val;
                    }
                    else {
                        sys.b=bu.val;
                    }
                    sys.ticks = SDL_GetTicks64();
                }    
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
    if (cpKIsPressed(SDLK_ESCAPE)){
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