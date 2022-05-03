#include "program.hpp"

#include <SDL2/SDL.h>

namespace pr
{
    void program::start()
    {
        renderer.load_height_map(256, 256, "../res/hmap2.png");
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
            auto state = SDL_GetKeyboardState(nullptr);
            if(state[SDL_SCANCODE_W])
                renderer.camera_pos.y += 0.0001f;
            if(state[SDL_SCANCODE_S])
                renderer.camera_pos.y -= 0.0001f;
        }
        renderer.close_rendering();
    }
}
