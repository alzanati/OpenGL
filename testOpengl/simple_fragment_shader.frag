#version 150

in  vec3 new_colors;
varying out vec4 fragColor;

void main() 
{
    fragColor = vec4( new_colors, 1.0 ); 
}