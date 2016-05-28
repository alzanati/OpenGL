#version 300 es

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color_in;
out vec3 color_in_frag;

void main()
{
    color_in_frag = color_in;
    gl_Position = vec4( position, 1.0 );
}
