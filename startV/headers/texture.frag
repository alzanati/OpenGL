#version 150

in vec3 Color;
in vec2 Texcoord;

uniform sampler2D tex;
uniform mat4 rotate;

out vec4 outColor;

void main()
{
    outColor = texture2D(tex, Texcoord) * vec4(Color, 1.0);
}
