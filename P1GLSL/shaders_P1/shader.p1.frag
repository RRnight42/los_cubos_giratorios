#version 330 core

uniform sampler2D colorTex;

in vec3 vPos;
in vec3 vNorm;

out vec4 outColor;

void main()
{
    // Obtenemos el color de la textura utilizando las coordenadas X e Y de la posición

    vec2 texCoord = vPos.xy;
    vec4 texColor = texture(colorTex, texCoord);

    // Descartar fragmentos cuyo color en la textura sea negro o similar , se puede variar el valor para conseguir otro resultado
    
    if (texColor.r == 0 && texColor.g == 0 && texColor.b == 0) {
        discard;
    }

    // Si el índice de la primitiva es impar, colorear con la normal, si es par, usar la textura

    if (gl_PrimitiveID % 2 != 0) {
        outColor = vec4(vNorm, 1.0);  
    } else {
        outColor = texColor;
    }

}
