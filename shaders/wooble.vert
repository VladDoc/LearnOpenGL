#version 330 core
layout (location = 0) in vec4 aPos;
uniform float time;

out vec2 TexCoord;

vec2 RotateUV(in vec2 uv, in float delta)
{
    return vec2(
        uv.x*cos(delta) - uv.y*sin(delta),
        uv.y*cos(delta) + uv.x*sin(delta)
    );
}

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0, 1.0);

    TexCoord = aPos.zw;
}

