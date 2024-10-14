#version 330 core

uniform sampler2D colorTex;
out vec4 outColor;


in vec3 vPos;
in vec3 vNorm;

void main()
{
   //outColor =  vec4(vPos.xy , 0 ,0);
   //outColor = vec4(vNorm , 0);

      if (gl_PrimitiveID % 2 != 0) {
      outColor = vec4(vNorm, 1.0);
        
    } else {
        outColor = texture(colorTex, vPos.xy);
        
    }

}
