
#version 330 core
in vec2 TexCoord;
uniform sampler2D texture0;
uniform float time;

out vec4 FragColor;

vec2 RotateUV(in vec2 uv, in float delta)
{
    return vec2(
        uv.x*cos(delta) - uv.y*sin(delta),
        uv.y*cos(delta) + uv.x*sin(delta)
    );
}

void main()
{
    vec2 coords = TexCoord;//vec2(TexCoord.x + sin(time), TexCoord.y);

    FragColor = mix(texture(texture0, coords), vec4(0, 1, 0, 1), 0);// sin(time) / 3);
}
