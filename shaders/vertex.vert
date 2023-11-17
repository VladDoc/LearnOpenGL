#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1

out vec3 ourColor; // output a color to the fragment shader

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
    vec3 p = aPos.xyz;
    vec2 uv = RotateUV(p.xy, time);

    gl_Position = vec4(uv, p.z, 1.0);
    ourColor = aColor; // set ourColor to the input color we got from the vertex data
} 
