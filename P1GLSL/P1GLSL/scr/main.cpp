﻿#include "BOX.h"
#include <IGL/IGlib.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
using namespace IGlib;
using namespace std;
using namespace glm;


//Idenficadores de los objetos de la escena
int objId =-1;
// añadimos un segundo cubo
int obj2Id = -1;
// añadimos un tercer cubo para bezier
int obj3Id = -1;

//vectores de la camara
vec3 COP = vec3(0.0f, 0.0f, 6.0f);
vec3 LookAt = vec3(0.0f, 0.0f, -1.0f);
vec3 VUP = vec3(0.0f, 1.0f, 0.0f);
float cameraSpeed = 0.1f;
float rotationSpeed = 0.05f;


//Camara con el raton

float horizontalAngle = 0.0f;
float verticalAngle = 0.0f;
int lastMouseX = -1, lastMouseY = -1;


//Declaración de CB
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMotionFunc(int x, int y);


int main(int argc, char** argv)
{
	locale::global(locale("spanish"));// acentos ;)
	if (!init("../shaders_P1/shader.v7.vert", "../shaders_P1/shader.p1.frag"))
		return -1;
   
	//Se ajusta la cámara
	//Si no se da valor se cojen valores por defecto
	
	float n = 1.;
	float f = 10.;
	mat4 model = mat4(1.0);
	mat4 view = mat4(1.0);
	mat4 proj = mat4(0.0);

	view[3].z = -6.f;


	proj[0].x = 1.732;
	proj[1].y = 1.732;
	proj[2][2] = (f + n) / (n - f);
	proj[2][3] = -1.f;
    proj[3].z = 2.f * f * n / (n - f);
	
	setProjMat(proj);
	setViewMat(view);

	//Creamos el objeto que vamos a visualizar
	objId = createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex, 
			cubeVertexPos, cubeVertexColor, cubeVertexNormal,cubeVertexTexCoord, cubeVertexTangent);

	obj2Id = createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex,
		cubeVertexPos, cubeVertexColor, cubeVertexNormal, cubeVertexTexCoord, cubeVertexTangent);

	//una matriz por objeto
	mat4 modelMat = mat4(1.0f);
	mat4 model2Mat = mat4(1.0f);


	setModelMat(objId, modelMat);
	
	setModelMat(obj2Id, model2Mat);
	
	//Incluir texturas aquí.
	
	addColorTex(objId, "../img/estrellas.png");

  //CBs
  setResizeCB(resizeFunc);
  setIdleCB(idleFunc);
  setKeyboardCB(keyboardFunc);
  setMouseCB(mouseFunc);
  setMouseMoveCB(mouseMotionFunc);

	
	//Mainloop
	mainLoop();
	destroy();
	return 0;
}

// hemos creado un metodo que permite devolver una matriz de rotacion en cualquier eje
mat4 createRotationMatrix(char axis, float angle)
{
	float cosAngle = cos(angle);
	float sinAngle = sin(angle);

	mat4 rot = mat4(1.0f);  // Matriz identidad

	switch (axis)
	{
	case 'X':  // Rotación en el eje X
		rot[1][1] = cosAngle;  // m[1][1]
		rot[1][2] = -sinAngle; // m[1][2]
		rot[2][1] = sinAngle;  // m[2][1]
		rot[2][2] = cosAngle;  // m[2][2]
		break;

	case 'Y':  // Rotación en el eje Y
		rot[0][0] = cosAngle;  // m[0][0]
		rot[0][2] = sinAngle;  // m[0][2]
		rot[2][0] = -sinAngle; // m[2][0]
		rot[2][2] = cosAngle;  // m[2][2]
		break;

	case 'Z':  // Rotación en el eje Z
		rot[0][0] = cosAngle;  // m[0][0]
		rot[0][1] = -sinAngle; // m[0][1]
		rot[1][0] = sinAngle;  // m[1][0]
		rot[1][1] = cosAngle;  // m[1][1]
		break;
	}

	return rot;
}
mat4 orbitalSpinAxis(char axis ,float angle) {
	// Matriz identidad inicial
	mat4 model = mat4(1.0f);

	// Trasladamos el objeto a una distancia fija del origen (centro de la órbita)
	float orbitRadius = 3.0f;

	// vector de desplazamiento con el radio de la orbita 
	model = translate(model, vec3(orbitRadius, 0.0f, 0.0f));
	
	model = createRotationMatrix(axis, angle) * model;

	return model;



}

mat4 createViewMatrix(vec3 CoP, vec3 LookAt, vec3 VUP) {
	
	// Direcciones , como en modelado geometrico
	vec3 N = normalize(CoP - LookAt);  // −lookAt/|lookAt| , teniendo en cuenta la posibilidad de que la camara no este en el origen

	vec3 Vaux = VUP - dot(VUP ,N)* N;  // VUP - (VUP · N)N
	vec3 V = normalize(Vaux);
	vec3 U = normalize(cross(V, N));    // V x N

	// Matriz de rotación de la cámara
	mat4 rotacion = mat4(1.0f);
	rotacion[0] = vec4(U, 0.0f);  // Eje U como primera fila
	rotacion[1] = vec4(V, 0.0f);  // Eje V como segunda fila
	rotacion[2] = vec4(-N, 0.0f); // -N como tercera fila (cámara mira en la dirección -Z)

	
	mat4 traslacion = mat4(1.0f);
	traslacion[0][3] = -COP.x;
	traslacion[1][3] = -COP.y;
	traslacion[2][3] = -COP.z;


	// Multiplicamos rotación y traslación para obtener la matriz de vista
	return rotacion * traslacion;
}

mat4 createProjMatrix(int width , int height , float anguloEnGrados){

	// Ajusta el aspect ratio al tamaño de la ventana

	float aspectRatio = (float)width / (float)height;
	float anguloApertura = anguloEnGrados * (pi<float>()/180); // RADIANES ,  usamos pi de glm


	//Mismos calculos de la matriz de proyeccion , pero cambiamos las componentes [0][0] , [1][1] : adaptamos el calculo del ratio

	float n = 1.;
	float f = 10.;

	mat4 proj = mat4(0.0);

	proj[0].x = 1.0f / (aspectRatio * tan(anguloApertura / 2.0f));
	proj[1].y = 1.0f / tan(anguloApertura / 2.0f);
	proj[2][2] = (f + n) / (n - f);
	proj[2][3] = -1.f;
	proj[3].z = 2.f * f * n / (n - f);

	return proj;

}



float clampValue(float x, float min, float max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}


void resizeFunc(int width, int height)
{
	// usamos nuestro metodo para crear una matriz de proyeccion
	setProjMat(createProjMatrix(width,height ,45.0f));
	
}

void idleFunc()
{
	static float angle = 0;
	angle =(angle < pi<float>()*2)? angle + 0.01f : 0.0f;

	// ESTE rot ES EL QUE VIMOS EN CLASE , NO ES ILEGAL
	mat4 rot  = rotate(mat4(1.0f), angle, vec3(1.0f, 1.0f, 0.0f));

	// Como rota sobre el eje Y , cambiamos la componentes XZ y dejamos la Y a 0
	mat4 rot2 = createRotationMatrix('Y',angle);

	mat4 orbita = orbitalSpinAxis('Y' ,angle);

	setModelMat(objId,rot);
	setModelMat(obj2Id, orbita * rot2);

}

void updateViewMatrix()
{
	//prohibido usar lookAt
	mat4 view = createViewMatrix(COP ,LookAt ,VUP);
	setViewMat(view);
}

void keyboardFunc(unsigned char key, int x, int y)
{


	switch (key) {

	case 'w':

		COP += cameraSpeed * LookAt;
		break;

	case 's':
		COP -= cameraSpeed * LookAt;
		break;

	case 'a':
		// producto vectorial de los dos vectores de la camara , hallamos el vector correspondiente y lo multiplicamos por la velocidad de la camara
		COP -= normalize(cross(LookAt, VUP)) * cameraSpeed;
		break;

	case 'd':
		// mismo caso que la a , pero en positivo(right)
		COP += normalize(cross(LookAt, VUP)) * cameraSpeed;
		break;

	case 'q':
		// Rotar cámara hacia la izquierda en el eje Y
	{
		// Generamos una matriz de rotación en el eje Y usando createRotationMatrix
		mat4 rotationMat = createRotationMatrix('Y', rotationSpeed);

		// Actualizamos la posición de la cámara COP respecto al punto de interés (LookAt)
		vec3 direction = COP - LookAt;  
		direction = vec3(rotationMat * vec4(direction, 0.0f));  

		COP = LookAt + direction;  
	}
	break;

	case 'e':
		// Rotar cámara hacia la derecha en el eje Y
	{
		// Generamos una matriz de rotación en el eje Y usando createRotationMatrix con ángulo negativo
		mat4 rotationMat = createRotationMatrix('Y', -rotationSpeed);

	
		vec3 direction = COP - LookAt;
		direction = vec3(rotationMat * vec4(direction, 0.0f)); 

		COP = LookAt + direction;  // Actualizamos la nueva posición de la cámara
	}
	break;
	}

	updateViewMatrix();
}

void mouseFunc(int button, int state, int x, int y)
{
	if (button == 0) cout << "izquierda" << endl;
	if (button == 1) cout << "rueda" << endl;
	if (button == 2) cout << "derecha" << endl;
	

	cout << "en la posición " << x << " " << y << endl << endl;
}

void mouseMotionFunc(int x, int y)
{
	if (lastMouseX == -1 || lastMouseY == -1)
	{
		// Inicializamos las posiciones del ratón si es la primera vez
		lastMouseX = x;
		lastMouseY = y;
		return;
	}

	// vectores del raton para hallar el desplazamiento
	int deltaX = x - lastMouseX;
	int deltaY = y - lastMouseY;

	// integracion numerica de la camara (actualizar valores de rotacion)
	horizontalAngle += deltaX * 0.005f;
	verticalAngle += deltaY * 0.005f;

	// Limitamos el ángulo vertical para no rotar más de 90 grados arriba o abajo
	verticalAngle = clampValue(verticalAngle, -1.5708f, 1.5708f);

	// Calculamos la nueva dirección de la cámara
	vec3 direction = vec3(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle));
	direction.x = cos(verticalAngle) * sin(horizontalAngle);
	direction.y = sin(verticalAngle);
	direction.z = cos(verticalAngle) * cos(horizontalAngle);

	// Calculamos la nueva posición "Right" y "Up" (para calcular la matriz de vista)
	vec3 right = vec3(sin(horizontalAngle - 1.5708f), 0, cos(horizontalAngle - 1.5708f));
	vec3 up = cross(right, direction);

	// Actualizamos el LookAt de la cámara
	LookAt = COP + direction;

	// Finalmente, actualizamos la matriz de vista
	updateViewMatrix();

	// Actualizamos las posiciones previas del ratón
	lastMouseX = x;
	lastMouseY = y;
}




