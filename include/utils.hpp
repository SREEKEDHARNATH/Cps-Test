#pragma once

#include "api.hpp"
class button{
    cpImage nonglow[4];
    cpImage glow[4];
    cpImage bg;
    void (*cb)();
    public:
        bool pressed=false;
        cpText t;
        bool select=false;
        bool strictglow=false;
        int val;
        button();
        button(cpFont& f, const char* txt, int x,int y,int v,bool small=false,void (*callback)()=nullptr);
        void set(cpFont& f, const char* txt, int x,int y,int v,bool small=false,void (*callback)()=nullptr);
        void draw();
        bool haspressed();
};