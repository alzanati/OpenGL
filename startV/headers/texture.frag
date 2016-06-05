#version 150

in vec3 Color;
in vec2 Texcoord;

uniform sampler2D tex;
uniform sampler2D tex1;

uniform int state;

out vec4 outColor;

void main()
{
    if (state == 0)
        outColor = texture2D(tex, Texcoord) * vec4(Color, 1.0).rgba;
    else if(state == 1)
        outColor = texture2D(tex1, Texcoord) * vec4(Color, 1.0);
}
