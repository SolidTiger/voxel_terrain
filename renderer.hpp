#ifndef RENDERER_H
#define RENDERER_H

#include <cstddef>
#include <thread>
#include <vector>

namespace pr
{
    struct color
    {
        std::uint8_t r, g, b, a;
        constexpr color() noexcept : r(0), g(0), b(0), a(0) {}
        constexpr color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) noexcept : r(r), g(g), b(b), a(a) {}
    };

    struct window_renderer
    {
        static constexpr std::size_t WINDOW_W = 640, WINDOW_H = 480;
        static constexpr std::size_t MAP_W = 256, MAP_H = 256;
        std::vector<float> height_map;
        bool not_closed = true;
        std::thread render_thread;
        /*
            Init rendering and start rendering thread
        */
        void start_rendering();
        void close_rendering();

        void load_height_map(const char* path);
    };
}

#endif