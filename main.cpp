#include "program.hpp"

#include <SDL2/SDL.h>

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    pr::program program;
    program.start();

    SDL_Quit();
}
