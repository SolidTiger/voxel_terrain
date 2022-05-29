#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <thread>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstddef>
#include <cstdint>

constexpr float TERRAIN_HEIGHT_SCALE = 200;
int h, w;

void load_hmap(std::vector<float> &v)
{
    std::string hmap_path;

    std::cout << "Input heightmap path\n";
    std::cin >> hmap_path;

    SDL_Surface *tmp = IMG_Load(hmap_path.c_str());
    if (tmp == nullptr)
        std::cout << "miep\n";
    SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32); // Probably a bad idea
    SDL_Surface *loaded_image = SDL_ConvertSurface(tmp, format, 0);
    SDL_FreeSurface(tmp);
    SDL_FreeFormat(format);

    v.resize(loaded_image->w * loaded_image->h);

    for (std::size_t i = 0; i < loaded_image->w * loaded_image->h; i++)
    {
        v[i] = float(*(((std::uint8_t *)(loaded_image->pixels)) + i * 4)) / 255.0f * TERRAIN_HEIGHT_SCALE;
    }
    h = loaded_image->h;
    w = loaded_image->w;
    SDL_FreeSurface(loaded_image);
}

// Refrence https://en.wikipedia.org/wiki/Digital_differential_analyzer_(graphics_algorithm)
void bake_shadows(const std::vector<float> &hmap, std::vector<std::uint8_t> &smap, const glm::vec3 &sun_dir)
{
    smap.resize(hmap.size());
    std::fill(smap.begin(), smap.end(), std::uint8_t(0));

    glm::vec3 inc;
    if (std::abs(sun_dir.x) >= std::abs(sun_dir.y))
    {
        inc = sun_dir * (1.0f / std::abs(sun_dir.x));
    }
    else
    {
        inc = sun_dir * (1.0f / std::abs(sun_dir.y));
    }

    for (std::size_t i = 0; i < hmap.size(); i++)
    {
        auto v1 = glm::vec3(
            i % w + 0.5f,
            i / h + 0.5f,
            0.0f);
        v1.z = hmap[int(v1.y) * w + int(v1.x)];

        auto v = v1 + inc; // skip first pixel

        while (v.x >= 0 && v.x < w && v.y >= 0 && v.y < h)
        {
            if (v.z < hmap[int(v.y) * w + int(v.x)])
            {
                smap[i] = 1;
                break;
            }
            v += inc;
        }
    }
}

void draw_line(std::vector<float>& hmap, std::vector<std::uint8_t> &smap, const glm::vec3 &sun_dir)
{
    smap.resize(hmap.size());
    std::fill(smap.begin(), smap.end(), std::uint8_t(1));

    glm::vec3 inc;
    if (std::abs(sun_dir.x) >= std::abs(sun_dir.y))
    {
        inc = sun_dir * (1.0f / std::abs(sun_dir.x));
    }
    else
    {
        inc = sun_dir * (1.0f / std::abs(sun_dir.y));
    }

    auto v1 = glm::vec3(
        500.0f,
        500.0f,
        0.0f);
    v1.z = hmap[int(v1.y) * w + int(v1.x)];

    auto v = v1 + inc; // skip first pixel

    while (v.x >= 0 && v.x < w && v.y >= 0 && v.y < h)
    {
        smap[int(v.y) * w + int(v.x)] = 0;
        v += inc;
    }
}

void save_shadows(const std::vector<std::uint8_t> &smap)
{
    std::ofstream f("shadowmap.ppm");
    f << "P1\n"
      << w << ' ' << h << '\n';
    for (std::size_t i = 0; i < smap.size(); i++)
    {
        f << int(smap[i]) << " ";
    }
    f.close();
}

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);

    std::vector<float> hmap;
    std::vector<std::uint8_t> smap;

    glm::vec3 sun_dir;
    std::cout << "Input sun dir 3 floats\n";
    std::cin >> sun_dir.x >> sun_dir.y >> sun_dir.z;

    load_hmap(hmap);
    bake_shadows(hmap, smap, sun_dir);
    //draw_line(hmap, smap, sun_dir);
    save_shadows(smap);

    SDL_Quit();
}