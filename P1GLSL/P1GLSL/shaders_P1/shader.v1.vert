#version 330 core

  uniform mat4 modelViewProj;

in vec3 inPos;		

void main()

{

	gl_Position =  modelViewProj * vec4(inPos , 1.);
}
