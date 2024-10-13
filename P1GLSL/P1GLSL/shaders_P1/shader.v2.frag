#version 330 core

out vec4 outColor;


void main()
{

if(mod(gl_PrimitiveID,2)==0){
outColor = vec4(1.0);
}else{
outColor = vec4(vec2(0.0) , vec2(1.0))
}

	outColor = vec4(0.0 , 1.0 ,vec2(0.0)) ;   
}
