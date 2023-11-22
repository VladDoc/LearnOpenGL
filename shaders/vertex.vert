#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform float time;

vec2 RotateUV(in vec2 uv, in float delta)
{
    return vec2(
        uv.x*cos(delta) - uv.y*sin(delta),
        uv.y*cos(delta) + uv.x*sin(delta)
    );
}

void main()
{
    vec2 uv = RotateUV(aPos.xy, time);

    gl_Position = vec4(uv, aPos.z, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
}
