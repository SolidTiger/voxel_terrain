#include "program.hpp"

#include <SDL2/SDL.h>

namespace pr
{
    void program::start()
    {
        renderer.load_height_map(1024, 1024, "../res/hmap6.png", "../res/colormap51.png", "../shadow_baking/build/shadowmap.ppm");
        renderer.start_rendering();
        SDL_Event event_sdl;
        bool no_close_event = true;
        auto start = SDL_GetTicks();

        while (no_close_event)
        {
            auto dtms = SDL_GetTicks() - start;
            start = SDL_GetTicks();
            float dt = float(dtms) / 1000.0f;

            while (SDL_PollEvent(&event_sdl))
            {
                if (event_sdl.type == SDL_QUIT || event_sdl.key.keysym.sym == SDLK_ESCAPE)
                    no_close_event = false;
            }
            auto state = SDL_GetKeyboardState(nullptr);
            if (state[SDL_SCANCODE_SPACE])
                renderer.camera_pos.y += 50 * dt;
            if (state[SDL_SCANCODE_LCTRL])
                renderer.camera_pos.y -= 50 * dt;
            if (state[SDL_SCANCODE_E])
                renderer.camera_rotation += 2.f * dt;
            if (state[SDL_SCANCODE_Q])
                renderer.camera_rotation -= 2.f * dt;
            if (state[SDL_SCANCODE_W])
            {
                glm::vec2 dir = glm::vec2(0, 1);
                float r = renderer.camera_rotation;
                const glm::mat2 rotation = glm::mat2(
                    glm::cos(r), -glm::sin(r),
                    glm::sin(r), glm::cos(r));
                dir = rotation * dir;
                renderer.camera_pos.x += dir.x * 50 * dt;
                renderer.camera_pos.z += dir.y * 50 * dt;
            }
            if (state[SDL_SCANCODE_S])
            {
                glm::vec2 dir = glm::vec2(0, 1);
                float r = renderer.camera_rotation;
                const glm::mat2 rotation = glm::mat2(
                    glm::cos(r), -glm::sin(r),
                    glm::sin(r), glm::cos(r));
                dir = rotation * dir;
                renderer.camera_pos.x -= dir.x * 50 * dt;
                renderer.camera_pos.z -= dir.y * 50 * dt;
            }

            if(renderer.camera_pos.y < renderer.hmap.get_point(renderer.camera_pos.x, renderer.camera_pos.z) * renderer.terrain_scale + 1.0f)
                renderer.camera_pos.y = renderer.hmap.get_point(renderer.camera_pos.x, renderer.camera_pos.z) * renderer.terrain_scale + 1.0f;
        }
        renderer.close_rendering();
    }
}
