#ifndef PROGRAM_H
#define PROGRAM_H
#include "renderer.hpp"

namespace pr
{
    struct program
    {
        window_renderer renderer;
        void start();
    };
}

#endif