#include <cstdlib>
#include <format>
#include "window.hpp"
#include "SDL.h"
#include "SDL_events.h"
#include "SDL_image.h"
#include "SDL_keyboard.h"
#include "SDL_keycode.h"
#include "SDL_mixer.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_scancode.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"
#include "SDL_ttf.h"
#include "SDL_video.h"
#include "api.hpp"
#include "listeners.hpp"
#include "utils.hpp"
#include <string>
#include <utility>
#include <vector>

bool init();
void handleinputs(bool& running);

SDL_Scancode anyKeyPressed(){
    for (int i=(int)SDL_SCANCODE_A;i<=(int)SDL_SCANCODE_Z;i++){
        if (cpKIsPressed((SDL_Scancode)i)){
            return (SDL_Scancode)i;
        }
    }
    for (int i=(int)SDL_SCANCODE_1;i<=(int)SDL_SCANCODE_0;i++){
        if (cpKIsPressed((SDL_Scancode)i)){
            return (SDL_Scancode)i;
        }
    }
    return (SDL_Scancode)0;
}
struct cpsSystem{
    int sec=5;
    int b=0;
    SDL_Keycode k=(SDL_Keycode)0;
    Uint64 ticks;
    cpImage clickarea;
    cpText t;
    cpText cps;
    cpText cpst;
    cpText tt;
    cpText timerem;
    cpImage blsq;
    std::vector<std::pair<short, short>> sqs;
    int clicks=0;
    int fps=240;
    float cpsscore=0;
    float peakscore=0;

    cpsSystem(cpFont& f) : timerem(f,"0"),blsq(0,255,255),clickarea(100,100,100),t(f,"0"), cps(f,"0"), cpst(f,"Cps"), tt(f,"Clicks"){
        clickarea.r.x=cpGetW()/5.0f;
        clickarea.r.y=cpGetH()/5.0f;
        clickarea.r.w=cpGetW()/1.5f;
        clickarea.r.h=cpGetH()/1.5f;
        tt.i.r.x=60;
        cps.i.r.x=200;
        cpst.i.r.x=250;
        blsq.r.w=16;
        blsq.r.h=16;
        timerem.i.r.x=cpGetW()-100;
    }
    bool update(cpFont& f){
        static auto start = SDL_GetTicks64();
        static auto remsec=sec;
        static cpSound click("assets/click.mp3");
        if (remsec==0){
            remsec=sec;
            start = SDL_GetTicks64();
        }
        if (remsec==sec){
            timerem.set(f, std::to_string(remsec).c_str());
        }
        if (SDL_GetTicks64()-start > 1000){
            remsec--;
            start = SDL_GetTicks64();
            timerem.set(f, std::to_string(remsec).c_str());
        }
        if (b == 0 && k != 0){
            auto bo=cpKIsPressed(SDL_GetScancodeFromKey(k));
            clicks+=(int)bo;
            if (bo){
                click.play();
            }
        }
        else if (k==0 && b != 0){
            SDL_Rect mouse = {cpMGetX(),cpMGetY(),1,1};
            if (SDL_HasIntersection(&clickarea.r, &mouse)){
                auto bo=cpMIsPressed(b);;
                clicks+=(int)bo;
                sqs.push_back({mouse.x,mouse.y});
                if (bo){
                    click.play();
                }
            }
        }
        t.set(f, std::to_string(clicks).c_str());
        auto st=std::format("{}", (clicks/(float)(SDL_GetTicks64()-ticks)/1000));
        auto dt=st.find(".");
        if (dt != st.npos){
            st=st.substr(0,dt+2);
        }
        cps.set(f, st.c_str());;

        auto cp=std::stof(st);
        peakscore=(cp>peakscore ? cp : peakscore);
        if ((ticks + sec*1000) < (SDL_GetTicks64()-(fps/1000))){
            cpsscore=cp;
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
        timerem.i.draw();
        if (k==0 && b != 0){
            clickarea.draw();
        }
        for (auto& sq : sqs){
            SDL_Rect s={sq.first,sq.second,8,8};
            blsq.r=s;
            blsq.draw();
        }
    }
    void reset(){
        k=(SDL_Keycode)0;
        b=0;
        clicks=0;
        sqs.clear();
    }
    void clearresult(){
        cpsscore=0;
        peakscore=0;
    }
};
void cpRun(){
    if (!init()){
        LOG();
        return;
    }

    cpFont f("assets/font.ttf");
    constexpr int size=12;
    button buttons[size];
    buttons[0].set(f,"Left Click",100,100,1);
    buttons[1].set(f,"Middle Click",500,100,2);
    buttons[2].set(f,"Right Click",900,100,3);

    buttons[3].set(f,"Space",150,300,SDLK_SPACE);
    {
        int x=500;
        int y=325;
        int gap=80;
        buttons[4].set(f,"W",x,y,SDLK_w,true);
        buttons[5].set(f,"A",x-gap,y+gap,SDLK_a,true);
        buttons[6].set(f,"S",x,y+(2*gap),SDLK_s,true);
        buttons[7].set(f,"D",x+gap,y+gap,SDLK_d,true);
        
        x=800;
        y=325;
        buttons[8].set(f,"^",x,y,SDLK_UP,true);
        buttons[9].set(f,"<",x-gap,y+gap,SDLK_LEFT,true);
        buttons[10].set(f,"v",x,y+(2*gap),SDLK_DOWN,true);
        buttons[11].set(f,">",x+gap,y+gap,SDLK_RIGHT,true);
    }
    
    button time[3];
    time[0].set(f, "1 sec", 100, 600, 1,true);
    time[1].set(f, "5 sec", 200, 600, 1,true);
    time[2].set(f, "10 sec", 300, 600, 1,true);
    
    cpsSystem sys(f);

    bool running = true;
    float fps=240;
    bool start=false;
    sys.ticks=0;

    int i=-1;
    cpText info(f,"Play");
    SDL_ShowWindow(cpGetWindow());
    while (running) {
        handleinputs(running);
        SDL_RenderClear(cpGetRenderer());
        if (start){
            start=sys.update(f);
            sys.draw();
            if (!start){
                sys.reset();
                std::string peak = std::format("{:.1f}",sys.peakscore) + std::string(" cps");
                if (sys.cpsscore < 2){
                    info.set(f, (std::string("Do you know how to click/press? and your peak score is: ") + peak).c_str());
                }
                else if(sys.cpsscore < 4){
                    info.set(f,(std::string("Just started to click/press right and your peak score is: ")+peak).c_str());
                }
                else if (sys.cpsscore < 6){
                    info.set(f, (std::string("Great a little experienced and your peak score is: ")+peak).c_str());
                }
                else if (sys.cpsscore < 8){
                    info.set(f, (std::string("nice and your peak score is: ")+peak).c_str());
                }
                else if (sys.cpsscore < 10){
                    info.set(f, (std::string("You have been grinding a lot and your peak score is: ")+peak).c_str());
                }
                else {
                    info.set(f, (std::string("auto clicker, cheater! and your peak score is: ") + peak).c_str());
                }
                sys.clearresult();
            }
        }
        else {
            if (cpKIsPressed(SDLK_BACKSPACE)){
                i=-1;
                for (int j=0;j<size;j++){
                    buttons[j].select=false;
                }
            }
            if (cpKIsPressed(SDLK_TAB)){
                if (i==-1){
                    i=0;
                }
                else {
                    buttons[i].select=false;
                    i++;
                    if (i == size){
                        i=0;
                    }
                }
            }
            if (i != -1){
                buttons[i].select=true;
            }
            for (auto& bu : buttons){
                bu.draw();
            }
            for (auto& t : time){
                t.draw();
            }
            info.i.draw();
        }
        SDL_RenderPresent(cpGetRenderer());
        if (!start && ((sys.ticks + 3000) < SDL_GetTicks64())){
            for (auto& bu : buttons){
                if (bu.haspressed()){
                    start=true;
                    if (bu.val > 3){
                        sys.k=(SDL_Keycode)bu.val;
                    }
                    else {
                        sys.b=bu.val;
                    }
                    sys.ticks = SDL_GetTicks64();
                }    
            }
            for (int t=0;t<3;t++){
                if (time[t].haspressed()){
                    time[0].strictglow=false;
                    time[1].strictglow=false;
                    time[2].strictglow=false;
                    time[t].strictglow=true;
                    sys.sec=std::stoi(time[t].t.getText());
                    break;
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