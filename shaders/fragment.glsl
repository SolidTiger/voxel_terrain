#version 330 core
out vec4 fragment_color;

in vec2 tex_cord;

uniform sampler2D my_texture;

void main()
{
    fragment_color = texture(my_texture, tex_cord);
}