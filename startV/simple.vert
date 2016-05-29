#version 300 es

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color_in;

uniform mat4 trans;
uniform int state;

out vec3 color_in_frag;
out vec3 v_positionsd;
out vec3 v_position;

void main()
{
    if(state == 0)
        gl_Position = trans * vec4( position, 1.0 );
    else
        gl_Position = vec4( position, 1.0 );
    v_position = position;
    color_in_frag = color_in;
}
