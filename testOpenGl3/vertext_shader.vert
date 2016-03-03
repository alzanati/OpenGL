#version 150

in vec2 in_coords;

void main()
{
	gl_Position = vec4(in_coords, 0.0, 1.0);
}
