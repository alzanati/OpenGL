#version 150

in vec2 in_coords;
uniform mat4 trans;

void main()
{
	vec4 new_coords = trans * vec4(in_coords, 0.0, 1.0);
	gl_Position = new_coords;
}
