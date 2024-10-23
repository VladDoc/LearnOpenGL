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
    vec4 color = vec4(1, 0, 0, 1);
    vec4 texel1 = texture(texture0, TexCoord);
    vec4 mixed = mix(texel1, color, 0.33);
    //redder = mix(redder, red, 0.33);
    //redder = mix(redder, red, 0.33);

    FragColor = mixed;
}
