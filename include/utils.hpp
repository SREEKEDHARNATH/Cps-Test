#pragma once

#include "api.hpp"
class button{
    cpImage nonglow[4];
    cpImage glow[4];
    cpImage bg;
    cpText t;
    void (*cb)();
    bool pressed=false;
    public:
        int val;
        button();
        button(cpFont& f, const char* txt, int x,int y,int v,bool small=false,void (*callback)()=nullptr);
        void set(cpFont& f, const char* txt, int x,int y,int v,bool small=false,void (*callback)()=nullptr);
        void draw();
        bool haspressed();
};