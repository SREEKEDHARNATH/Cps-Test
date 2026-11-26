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
        button(cpFont& f, const char* txt, int x,int y,void (*callback)());
        void draw();
        bool haspressed();
};