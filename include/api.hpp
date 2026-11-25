#pragma once

#include "SDL_render.h"
#include <memory>
class cpImage{
    std::shared_ptr<SDL_Texture> ptr;
    public:
        SDL_Rect r = {0,0,0,0};
        cpImage();
        cpImage(const char* name, bool autoset=true);
        cpImage(int r,int g,int b,bool autoset=true);
        cpImage(SDL_Texture* tex,bool autoset=true);
        void set(const char* name, bool autoset=true);
        void set(int r,int g,int b,bool autoset=true);
        void set(SDL_Texture* tex,bool autoset=true);
        void draw();
        void setdimensions();
        std::shared_ptr<SDL_Texture> get();
};