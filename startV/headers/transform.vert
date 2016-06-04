#version 300 es

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texcoord;

uniform mat4 rotate;

out vec3 Color;
out vec2 Texcoord;
out vec2 v_position;

void main()
{
    Color = color;
    Texcoord = texcoord;
    v_position = Texcoord;
    gl_Position = rotate * vec4(position, 0.f, 1.f);
}
