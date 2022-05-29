#version 330 core
layout (location = 0) in vec3 vertex_pos;
layout (location = 1) in vec2 vertex_tex_cord;

out vec2 tex_cord;

void main()
{
    gl_Position = vec4(vertex_pos, 1.0);
    tex_cord = vertex_tex_cord;
}