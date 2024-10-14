#version 330 core

in vec3 inPos;		

void main()

{

   float n = 1. ;
   float f = 10. ;
   mat4 model = mat4(1.0);
   mat4 view = mat4(1.0);
   mat4 proj = mat4(0.0);

   proj[0].x = 1.732;
   proj[1].y = 1.732;
   proj[2].zw = vec2((f+n)/(n-f) ,-1.);
   view[3].z =  2.* f * n/(n-f);

   



	gl_Position = proj * view * model * vec4(inPos , 1.);
}
