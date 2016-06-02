#version 300 es

in vec2 texcoord;

out vec3 Color;
out vec2 Texcoord;

void main()
{
    Texcoord = texcoord;
}
