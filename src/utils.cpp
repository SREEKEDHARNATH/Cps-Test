#include "utils.hpp"
#include "SDL_mouse.h"
#include "SDL_rect.h"
#include "listeners.hpp"

button::button(){

}
button::button(cpFont& f, const char* txt, int x,int y,int v,bool small,void (*callback)()){
    set(f,txt,x,y,v,small,callback);
}
void button::set(cpFont& f, const char* txt, int x,int y,int v,bool small,void (*callback)()){
    val=v;
    cb=callback;
    bg.set(50, 50,50);
    bg.r.x=x;
    bg.r.y=y;
    bg.r.h=64;
    if (small){
        bg.r.w=64;
    }
    else {
        bg.r.w=192;
    }
    int wi=3;
    for (int i=0;i<4;i++){
        nonglow[i].set(255, 255,255);
        nonglow[i].r.x=x;
        nonglow[i].r.y=y;
        if (i==2 || i==0){
            nonglow[i].r.w=wi;
            nonglow[i].r.h=bg.r.h;
            if (i){
                nonglow[i].r.x+=bg.r.w-wi;
            }
        }
        else {
            nonglow[i].r.w=bg.r.w;
            nonglow[i].r.h=wi;
            if (i%3){
                nonglow[i].r.y+=bg.r.h-wi;
            }
        }
    }
    for (int i=0;i<4;i++){
        glow[i].set(176, 5, 170);
        glow[i].r=nonglow[i].r;
    }
    t.set(f, txt);
    t.i.r.x=x+wi;
    t.i.r.y=y+wi;
    while ((t.i.r.w > (bg.r.w-wi)) || (t.i.r.h > (bg.r.h-wi))){
        t.i.r.w/=1.1;
        t.i.r.h/=1.1;
    }
    auto& r = t.i.r;
    while (((r.x*2)+r.w < (bg.r.x*2)+bg.r.w) && ((r.y*2)+r.h < (bg.r.x*2)+bg.r.w)){
        t.i.r.x+=1;
        t.i.r.y+=1;
    }
}
void button::draw(){
    pressed=false;
    bg.draw();
    SDL_Rect mouse = {cpMGetX(),cpMGetY(),1,1};
    if (SDL_HasIntersection(&mouse, &bg.r)){
        if (cpMIsPressed(((val <= 0 || val > 3) ? SDL_BUTTON_LEFT : val))){
            pressed=true;
            if (cb){
                cb();
            }
        }
        for (auto& i : glow){
            i.draw();
        }
    }
    else {
        for (auto& i : nonglow){
            i.draw();
        }
    }
    t.i.draw();
}
bool button::haspressed(){
    return pressed;
}