#pragma once

#include "SDL_render.h"
#include <memory>
#include "SDL_ttf.h"
#include "SDL_mixer.h"

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

class cpFont{
    std::shared_ptr<TTF_Font> ptr;
    public:
        cpFont();
        cpFont(const char* name, int ptsize=32);
        cpFont(TTF_Font* f);
        void set(const char* name, int ptsize=32);
        void set(TTF_Font* f);
        std::shared_ptr<TTF_Font> get();
};

class cpText{
    std::string text;
    public:
        cpImage i;
        cpText();
        cpText(cpFont& f,const char* txt);
        void set(cpFont& f,const char* txt);
        const char* getText();
};

class cpSound{
    std::shared_ptr<Mix_Chunk> ptr;
    public:
        int vol=50;
        cpSound();
        cpSound(const char* name);
        cpSound(Mix_Chunk* m);
        void set(const char* name);
        void set(Mix_Chunk* m);
        void play();
        std::shared_ptr<Mix_Chunk> get();
};