#include "program.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    pr::program program;
    program.start();

    SDL_Quit();
}
