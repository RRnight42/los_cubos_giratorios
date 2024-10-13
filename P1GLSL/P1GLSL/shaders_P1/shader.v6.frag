#version 330 core

uniform sampler2D colorTex;
out vec4 outColor;
in vec2 vTexCoord;

void main()
{
    //outColor = vec4(vTexCoord.xy, vec2(1));
    outColor = texture(colorTex, vTexCoord.xy);   

    if(outColor.g > 0.8) discard;
}
