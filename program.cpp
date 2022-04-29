#include "program.hpp"

#include <SDL2/SDL.h>

namespace pr
{
    void program::start()
    {
        renderer.load_height_map("../res/hmap.bmp");
        renderer.start_rendering();
        SDL_Event event_sdl;
        bool no_close_event = true;
        while (no_close_event)
        {
            while (SDL_PollEvent(&event_sdl))
            {
                if (event_sdl.type == SDL_QUIT || event_sdl.key.keysym.sym == SDLK_ESCAPE)
                    no_close_event = false;
            }
        }
        renderer.close_rendering();
    }
}
