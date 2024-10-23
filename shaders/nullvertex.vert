#version 330 core
layout (location = 0) in vec4 aPos;
uniform float time;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos.xy, 0, 1.0);

    TexCoord = aPos.zw;
}
