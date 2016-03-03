#version 150 

in  vec3 in_coords;
in  vec3 in_colors;
out vec3 new_colors;

void main() 
{
	new_colors = in_colors;
	mat3x3 rot_matrix = mat3x3( 0.707, 0.641, -0.299,
                                -0.707, 0.641, -0.299,
                                -0.000, 0.423,  0.906 );
    vec3 new_coords = in_coords * rot_matrix;
    gl_Position = vec4( new_coords, 1.0 );
}