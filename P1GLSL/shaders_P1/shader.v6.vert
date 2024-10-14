#version 330 core

uniform mat4 modelViewProj;

in vec3 inPos;	
in vec2 inTexCoord;
out vec2 vTexCoord;

void main()

{
    vTexCoord = inTexCoord;
	gl_Position =  modelViewProj * vec4(inPos , 1.);
}
