#version 330 core

uniform mat4 modelViewProj;
uniform mat4 modelView;
uniform mat4 normal;
in vec3 inPos;	
in vec3 inNormal;

out vec3 vPos;
out vec3 vNorm;

void main()

{
    vPos = (modelView * vec4(inPos, 1)).xyz;
	//vNorm = inverse(transpose(modelView)) * vec4(inNormal , 0); optimizacion la de abajo
	vNorm = (normal * vec4(inNormal, 0)).xyz;

	gl_Position =  modelViewProj * vec4(inPos , 1.);
}
