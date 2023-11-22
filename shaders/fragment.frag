#version 330 core

out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform float time;

vec2 RotateUV(in vec2 uv, in float delta)
{
    return vec2(
        uv.x*cos(delta) - uv.y*sin(delta),
        uv.y*cos(delta) + uv.x*sin(delta)
    );
}

float ChangeScale(in float value, in vec2 oldScale, in vec2 newScale)
{
    return ((value - oldScale.x) / (oldScale.y - oldScale.x)) * (newScale.y - newScale.x) + newScale.x;
}
  
void main()
{
    const vec2 sinRange = vec2(-1, 1);
    const vec2 uvRange = vec2(0, 1); 
    vec3 uv = vec3(ChangeScale( sin(time), sinRange, uvRange), 
                   ChangeScale( cos(time), sinRange, uvRange),
                   ChangeScale(-sin(time), sinRange, uvRange));
    
    //FragColor = vec4(normalize(ourColor * uv), 1);

    FragColor = texture(ourTexture, TexCoord) * vec4(normalize(ourColor * uv), 1);
}