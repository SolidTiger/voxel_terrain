#include "renderer.hpp"

#include <fstream>
#include <string>
#include <streambuf>
#include <iostream>
#include <algorithm>
#include <cmath>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

#include <glm/glm.hpp>

namespace // anonymous namespace
{
    std::vector<pr::color> pixel_buffer;
    GLuint screen_texture, shader_program, quad;

    void draw_vline(std::size_t x, std::size_t y1, std::size_t y2, pr::color color)
    {

        if (x >= pr::window_renderer::WINDOW_W)
            return;

        for (std::size_t y = y1; y < y2 + 1; y++)
        {
            if (y >= pr::window_renderer::WINDOW_W)
                break;
            pixel_buffer[y * pr::window_renderer::WINDOW_W + x] = color;
        }
    }

    void draw_terrain(const pr::window_renderer *renderer)
    {
        const int distance = 300;
        const float focus = pr::window_renderer::WINDOW_W / 2;

        for (int z = distance - 1; z > 0; z--)
        {
            auto left = glm::normalize(glm::vec2(-int(pr::window_renderer::WINDOW_W) / 2, focus)) * float(z);
            auto right = glm::normalize(glm::vec2(pr::window_renderer::WINDOW_W / 2, focus)) * float(z);
            auto dx = (right - left) / float(pr::window_renderer::WINDOW_W);

            for (int i = 0; i < pr::window_renderer::WINDOW_W; i++)
            {
                float height_on_screen = (-renderer->hmap.get_point(left.x, left.y)*80 + renderer->camera_pos.y) * focus / float(z) +
                                         float(pr::window_renderer::WINDOW_H) / 2;
                draw_vline(i, height_on_screen, pr::window_renderer::WINDOW_H - 1,
                           pr::color(255.0f * renderer->hmap.get_point(left.x, left.y),
                                     255.0f * renderer->hmap.get_point(left.x, left.y),
                                     255.0f * renderer->hmap.get_point(left.x, left.y),
                                     255));
                left += dx;
            }
        }
    }

    void clear_texture(){
        std::fill(pixel_buffer.begin(), pixel_buffer.end(), pr::color(0,0,0,255));
    }

    GLuint compile_shader(const char *path, const GLenum type)
    {
        std::ifstream t;
        t.open(path);
        std::string source((std::istreambuf_iterator<char>(t)),
                           std::istreambuf_iterator<char>());

        const GLuint shader = glCreateShader(type);
        const char *source_ptr = &source[0];
        glShaderSource(shader, 1, &source_ptr, NULL);
        glCompileShader(shader);

        t.close();

        return shader;
    }

    void create_shader_program(const char *vertex_path, const char *fragment_path)
    {
        GLuint vertex_shader = compile_shader(vertex_path, GL_VERTEX_SHADER);
        GLuint fragment_shader = compile_shader(fragment_path, GL_FRAGMENT_SHADER);

        shader_program = glCreateProgram();
        glAttachShader(shader_program, vertex_shader);
        glAttachShader(shader_program, fragment_shader);
        glLinkProgram(shader_program);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        GLint success;
        glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
            std::cout << infoLog << std::endl;
        }
    }

    void create_quad()
    {
        constexpr float vertices[] = {
            -1.0f, 1.0f, 0.0f, 0.0f, 0.0f,  // Top left
            -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, // Bottom left
            1.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // Top right
            1.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // Top right
            -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, // Bottom left
            1.0f, -1.0f, 0.0f, 1.0f, 1.0f   // Bottom right
        };

        GLuint VAO, VBO;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, 5 * 6 * sizeof(float), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        quad = VAO;
    }

    void create_texture()
    {
        glGenTextures(1, &screen_texture);
        glBindTexture(GL_TEXTURE_2D, screen_texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA,
            pr::window_renderer::WINDOW_W, pr::window_renderer::WINDOW_H,
            0, GL_RGBA, GL_UNSIGNED_BYTE, &pixel_buffer[0]);
    }

    void update_texture()
    {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, pr::window_renderer::WINDOW_W, pr::window_renderer::WINDOW_H, GL_RGBA, GL_UNSIGNED_BYTE, &pixel_buffer[0]);
    }

    void internal_loop(const pr::window_renderer *renderer)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_Window *window = SDL_CreateWindow(
            "voxel", 50, 50, pr::window_renderer::WINDOW_W, pr::window_renderer::WINDOW_H, SDL_WINDOW_OPENGL);

        SDL_GLContext context = SDL_GL_CreateContext(window);
        SDL_GL_SetSwapInterval(0); // Turn of vsync

        glewExperimental = GL_TRUE;

        glewInit();

        pixel_buffer.resize(pr::window_renderer::WINDOW_W * pr::window_renderer::WINDOW_H);

        create_quad();
        create_shader_program("../shaders/vertex.glsl", "../shaders/fragment.glsl");
        create_texture();

        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

        glUseProgram(shader_program);
        glBindTexture(GL_TEXTURE_2D, screen_texture);
        glBindVertexArray(quad);

        while (renderer->not_closed)
        {
            glClear(GL_COLOR_BUFFER_BIT);
            clear_texture();
            draw_terrain(renderer);
            update_texture();
            glDrawArrays(GL_TRIANGLES, 0, 6);
            SDL_GL_SwapWindow(window);
        }

        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
    }
}

namespace pr
{

    height_map::height_map() : w(0), h(0), data() {}

    height_map::height_map(std::size_t w, std::size_t h, const char *image_path) : w(w), h(h)
    {
        SDL_Surface *tmp = IMG_Load(image_path);
        SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32); // Probably a bad idea
        SDL_Surface *loaded_image = SDL_ConvertSurface(tmp, format, 0);
        SDL_FreeSurface(tmp);
        SDL_FreeFormat(format);

        data.resize(w * h);

        for (std::size_t i = 0; i < w * h; i++)
        {
            data[i] = float(*(((std::uint8_t *)(loaded_image->pixels)) + i * 4)) / 255.0f;
        }
        SDL_FreeSurface(loaded_image);
    }

    float height_map::get_point(std::size_t x, std::size_t z) const
    {
        x += w / 2;
        z = h / 2 - z;
        if (x >= w || z >= h)
            return 0.0;
        return data[z * w + x];
    }

    void window_renderer::start_rendering()
    {
        render_thread = std::thread(internal_loop, this);
    }

    void window_renderer::close_rendering()
    {
        not_closed = false;
        render_thread.join();
    }

    void window_renderer::load_height_map(std::size_t w, std::size_t h, const char *path)
    {
        hmap = height_map(w, h, path);
    }
}
