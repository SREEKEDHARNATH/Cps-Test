#include "api.hpp"
#include "SDL_pixels.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_ttf.h"
#include "window.hpp"
#include "SDL_image.h"

cpImage::cpImage(){

}
cpImage::cpImage(const char* name, bool autoset){
    set(name,autoset);
}
cpImage::cpImage(int r,int g,int b,bool autoset){
    set(r,g,b,autoset);
}
cpImage::cpImage(SDL_Texture* tex,bool autoset){
    set(tex,autoset);
}
void cpImage::set(const char* name, bool autoset){
    if (auto tex = IMG_LoadTexture(cpGetRenderer(),name)){
        ptr.reset(tex, SDL_DestroyTexture);
        if (autoset){
            setdimensions();
        }
    }
    else {
        LOG();
    }
}
void cpImage::set(int r,int g,int b,bool autoset){
    auto surf = SDL_CreateRGBSurface(0, 32,32,32,0,0,0,0);
    if (!surf){
        LOG();
        return;
    }
    auto colour = SDL_MapRGB(surf->format, r, g, b);
    SDL_FillRect(surf, NULL,colour);
    auto tex = SDL_CreateTextureFromSurface(cpGetRenderer(), surf);
    if (!tex){
        LOG();
        return;
    }
    ptr.reset(tex, SDL_DestroyTexture);
    if (autoset){
        setdimensions();
    }
}
void cpImage::set(SDL_Texture* tex,bool autoset){
    if (tex){
        ptr.reset(tex, SDL_DestroyTexture);
        if (autoset){
            setdimensions();
        }
    }
    else {
        LOGARGS("nullptr gigivenven as texture in cpImage set function");
    }
}
void cpImage::draw(){
    if (get().get()){
        if (r.w <= 0 || r.h <= 0){
            SDL_RenderCopy(cpGetRenderer(), get().get(),NULL,NULL);
        }
        else {
            SDL_RenderCopy(cpGetRenderer(), get().get(),NULL,&r);
        }
    }
}
void cpImage::setdimensions(){
    if (get().get()){
        SDL_QueryTexture(get().get(), NULL,NULL,&r.w,&r.h);
    }
    else {
        LOGARGS("Set Texture in cpImage first the call setdimensions");
    }
}
std::shared_ptr<SDL_Texture> cpImage::get(){
    return ptr;
}

cpFont::cpFont(){

}
cpFont::cpFont(const char* name, int ptsize){
    set(name,ptsize);
}
cpFont::cpFont(TTF_Font* f){
    set(f);
}
void cpFont::set(const char* name, int ptsize){
    auto t =TTF_OpenFont(name, ptsize);
    if (!t){
        LOG();
        return;
    }
    ptr.reset(t, TTF_CloseFont);
}
void cpFont::set(TTF_Font* f){
    if (f){
        ptr.reset(f, TTF_CloseFont);
    }
    else {
        LOGARGS("nullptr given as font in cpFont set function");
    }
}
std::shared_ptr<TTF_Font> cpFont::get(){
    return ptr;
}

cpText::cpText(){

}
cpText::cpText(cpFont& f,const char* txt){
    set(f,txt);
}
void cpText::set(cpFont& f,const char* txt){
    if (std::string(txt) == ""){
        LOGARGS("Text for cptext cant be empty");
        return;
    }
    if (!f.get().get()){
        LOGARGS("Font for cptext cant be invalid");
        return;
    }
    auto surf = TTF_RenderUTF8_Blended(f.get().get(), txt, {255,255,255,255});
    if (!surf){
        LOG();
        return;
    }
    auto tex=SDL_CreateTextureFromSurface(cpGetRenderer(), surf);
    SDL_FreeSurface(surf);
    if (!tex){
        LOG();
        return;
    }
    i.set(tex);
    if ((i.r.x+i.r.w) > cpGetW()){
        LOGARGS("Text is overlapping than window's width");
    }
    if ((i.r.y+i.r.h) > cpGetW()){
        LOGARGS("Text is overlapping than window's height");
    }
    
}
const char* cpText::getText(){
    return text.c_str();
}