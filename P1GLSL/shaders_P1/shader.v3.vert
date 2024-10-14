#version 330 core

  uniform mat4 modelViewProj;

in vec3 inPos;	
in vec3 inColor;
out vec3 vcolor;

void main()

{
    vcolor = inColor;
	gl_Position =  modelViewProj * vec4(inPos , 1.);
}
