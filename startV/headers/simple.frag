#version 150

in vec3 color_in_frag;
out vec4 color_out;


void main()
{
    color_out = vec4(color_in_frag, 1.0);
}
