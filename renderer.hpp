#ifndef RENDERER_H
#define RENDERER_H

#include <cstddef>
#include <cfloat>
#include <thread>
#include <vector>

#include <glm/glm.hpp>

namespace pr
{
    struct color
    {
        std::uint8_t r, g, b, a;
        constexpr color() noexcept : r(0), g(0), b(0), a(0) {}
        constexpr color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) noexcept : r(r), g(g), b(b), a(a) {}
    };

    struct height_map
    {
        std::size_t w, h;
        std::vector<float> data;
        std::vector<color> colormap;
        height_map();
        height_map(std::size_t w, std::size_t h, const char *heightmap_path, const char* colormap_path);
        float get_point(std::size_t x, std::size_t z) const;
        color get_point_color(std::size_t x, std::size_t z) const;
    };

    struct window_renderer
    {
        static constexpr std::size_t WINDOW_W = 640, WINDOW_H = 480;
        static constexpr std::size_t TEXTURE_W = 320, TEXTURE_H = 240;
        height_map hmap;
        glm::vec3 camera_pos = glm::vec3(0,100,0);
        float camera_rotation = 0.0f;
        bool not_closed = true;
        std::thread render_thread;
        /*
            Init rendering and start rendering thread
        */
        void start_rendering();
        void close_rendering();

        void load_height_map(std::size_t w, std::size_t h, const char *heightmap_path, const char *colormap_path);
    };
}

#endif