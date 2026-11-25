#include "api.hpp"
#include "SDL_pixels.h"
#include "SDL_render.h"
#include "SDL_surface.h"
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
        LOGARGS("nullptr given as texture in cpImage set function");
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
}
std::shared_ptr<SDL_Texture> cpImage::get(){
    return ptr;
}