#version 300 es

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color_in;

uniform mat4 trans;

out vec3 color_in_frag;

void main()
{
    gl_Position = trans * vec4( position, 1.0 );
    color_in_frag = color_in;
}
