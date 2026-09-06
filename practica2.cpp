//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá
//SHADER AMARILLO
static const char* vShaderAmarillo = "shaders/shaderamarillo.vert";
static const char* fShaderAmarillo = "shaders/shaderamarillo.frag";
//SHADER AZUL
static const char* vShaderAzul = "shaders/shaderazul.vert";
static const char* fShaderAzul = "shaders/shaderazul.frag";
// SHADER CAFE
static const char* vShaderCafe = "shaders/shadercafe.vert";
static const char* fShaderCafe = "shaders/shadercafe.frag";
// SHADER ROJO
static const char* vShaderRojo = "shaders/shaderrojo.vert";
static const char* fShaderRojo = "shaders/shaderrojo.frag";
// SHADER VERDE
static const char* vShaderVerde = "shaders/shaderverde.vert";
static const char* fShaderVerde = "shaders/shaderverde.frag";
//SHADER NEGRO
static const char* vShaderNegro = "shaders/shadernegro.vert";
static const char* fShaderNegro = "shaders/shadernegro.frag";
//SHADER MORADO
static const char* vShaderMorado = "shaders/shadermorado.vert";
static const char* fShaderMorado = "shaders/shadermorado.frag";

float angulo = 0.0f;

//color café/marrón en RGB : 0.478, 0.255, 0.067

using std::vector;

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0, 1, 2,
		1, 3, 2,
		3, 0, 2,
		1, 0, 3
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,  0.0f,	//0
		 0.5f, -0.5f,  0.0f,	//1
		 0.0f,  0.5f, -0.25f,	//2
		 0.0f, -0.5f, -0.5f,	//3
	};
	Mesh *piramidetriangular = new Mesh();
	piramidetriangular->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(piramidetriangular);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	unsigned int piramidecuadrangular_indices[] = {
		0, 3, 4,
		3, 2, 4,
		2, 1, 4,
		1, 0, 4,
		0, 1, 2,
		0, 2, 4
	};
	GLfloat piramidecuadrangular_vertices[] = {
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		 0.0f,  0.5f,  0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMesh(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}
//Función para una pirámide con forma de triángulo rectángulo
void CrearPiramideOblicua()
{
	unsigned int piramideoblicua_indices[] = {
		0, 3, 4,
		3, 2, 4,
		2, 1, 4,
		1, 0, 4,
		0, 1, 2,
		0, 2, 4
	};
	GLfloat piramideoblicua_vertices[] = {
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.0f,
	};
	Mesh* piramideoblicua = new Mesh();
	piramideoblicua->CreateMesh(piramideoblicua_vertices, piramideoblicua_indices, 15, 18);
	meshList.push_back(piramideoblicua);
}


//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	//--------------------------------LETRAS--------------------------- //Indice 0
	GLfloat vertices_letras[] = {	
			//X			Y			Z			R		G		B
		//----LETRA E
		//parte de arriba
			-0.9f,		0.4f,		0.0f,		1.0f,	0.0f,	0.0f,
			-0.5f,		0.4f,		0.0f,		1.0f,	0.0f,	0.0f,
			-0.5f,		0.2f,		0.0f,		1.0f,	0.0f,	0.0f,

			-0.9f,		0.4f,		0.0f,		1.0f,	0.0f,	0.0f,
			-0.9f,		0.2f,		0.0f,		1.0f,	0.0f,	0.0f,
			-0.5f,		0.2f,		0.0f,		1.0f,	0.0f,	0.0f,
		//Parte de en medio

			-0.9f,		0.2f,		0.0f,		1.0f,	0.0f,	0.0f,
			-0.7f,		0.2f,		0.0f,		1.0f,	0.0f,	0.0f,
			-0.7f,	   -0.2f,		0.0f,		1.0f,	0.0f,	0.0f,

			-0.9f,		0.2f,		0.0f,		1.0f,	0.0f,	0.0f,
			-0.9f,	   -0.2f,		0.0f,		1.0f,	0.0f,	0.0f,
			-0.7f,	   -0.2f,		0.0f,		1.0f,	0.0f,	0.0f,
		//parte abajo
			-0.9f,	   -0.2f,		0.0f,		1.0f,	0.0f,	0.0f,
			-0.5f,	   -0.2f,		0.0f,		1.0f,	0.0f,	0.0f,
			-0.9f,	   -0.4f,		0.0f,		1.0f,	0.0f,	0.0f,

			-0.9f,	   -0.4f,		0.0f,		1.0f,	0.0f,	0.0f,
			-0.5f,	   -0.2f,		0.0f,		1.0f,	0.0f,	0.0f,
			-0.5f,	   -0.4f,		0.0f,		1.0f,	0.0f,	0.0f,

		//medio E
			-0.7f,		0.1f,		0.0f,		1.0f,	0.0f,	0.0f,
			-0.7f,	   -0.1f,		0.0f,		1.0f,	0.0f,	0.0f,
			-0.5f,		0.1f,		0.0f,		1.0f,	0.0f,	0.0f,
							
			-0.7f,	   -0.1f,		0.0f,		1.0f,	0.0f,	0.0f,
			-0.5f,		0.1f,		0.0f,		1.0f,	0.0f,	0.0f,
			-0.5f,	   -0.1f,		0.0f,		1.0f,	0.0f,	0.0f,

		//----LETRA V
			-0.3f,		0.4f,		0.0f,		0.0f,	1.0f,	0.0f,
			-0.1f,		0.4f,		0.0f,		0.0f,	1.0f,	0.0f,
			 0.0f,		0.0f,		0.0f,		0.0f,	1.0f,	0.0f,

			-0.3f,		0.4f,		0.0f,		0.0f,	1.0f,	0.0f,
			-0.2f,		0.0f,		0.0f,		0.0f,	1.0f,	0.0f,
			 0.0f,		0.0f,		0.0f,		0.0f,	1.0f,	0.0f,

			-0.2f,		0.0f,		0.0f,		0.0f,	1.0f,	0.0f,
			-0.1f,	   -0.4f,		0.0f,		0.0f,	1.0f,	0.0f,
			 0.0f,		0.0f,		0.0f,		0.0f,	1.0f,	0.0f,
					
			 0.0f,		0.0f,		0.0f,		0.0f,	1.0f,	0.0f,
			-0.1f,	   -0.4f,		0.0f,		0.0f,	1.0f,	0.0f,
			 0.1f,	   -0.4f,		0.0f,		0.0f,	1.0f,	0.0f,

			 0.0f,		0.0f,		0.0f,		0.0f,	1.0f,	0.0f,
			 0.1f,	   -0.4f,		0.0f,		0.0f,	1.0f,	0.0f,
			 0.2f,		0.0f,		0.0f,		0.0f,	1.0f,	0.0f,
				
			 0.0f,		0.0f,		0.0f,		0.0f,	1.0f,	0.0f,
			 0.2f,		0.0f,		0.0f,		0.0f,	1.0f,	0.0f,
			 0.3f,		0.4f,		0.0f,		0.0f,	1.0f,	0.0f,

			 0.0f,		0.0f,		0.0f,		0.0f,	1.0f,	0.0f,
			 0.3f,		0.4f,		0.0f,		0.0f,	1.0f,	0.0f,
			 0.1f,		0.4f,		0.0f,		0.0f,	1.0f,	0.0f,

		//----LETRA A
		//Pata izquierda
			0.3f,	  -0.4f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.4f,	   0.0f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.6f,	   0.0f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.3f,	  -0.4f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.5f,	  -0.4f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.6f,	   0.0f,		0.0f,		0.0f,	0.0f,	1.0f,
		//Pata derecha
			0.6f,	   0.0f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.7f,	  -0.4f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.9f,	  -0.4f,		0.0f,		0.0f,	0.0f,	1.0f,
			//0.9f,	  -0.4f,		0.0f,		0.0f,	0.0f,	1.0f,
			//0.9f,	  -0.4f,		0.0f,		0.0f,	0.0f,	1.0f,
			//0.9f,	  -0.4f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.6f,	   0.0f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.8f,	   0.0f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.9f,	  -0.4f,		0.0f,		0.0f,	0.0f,	1.0f,
		//ZONA MEDIA
			0.4f,	   0.0f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.5f,	   0.0f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.5f,	   0.1f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.4f,	   0.0f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.5f,	   0.1f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.43f,	   0.1f,		0.0f,		0.0f,	0.0f,	1.0f,

			0.5f,	   0.0f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.7f,	   0.0f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.7f,	   0.1f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.5f,	   0.0f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.5f,	   0.1f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.7f,	   0.1f,		0.0f,		0.0f,	0.0f,	1.0f,

			0.7f,	   0.0f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.7f,	   0.1f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.8f,	   0.0f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.8f,	   0.0f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.7f,	   0.1f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.77f,	   0.1f,		0.0f,		0.0f,	0.0f,	1.0f,
		//ZONA MEDIA SUPERIOR
			0.43f,	   0.1f,		0.0f,		0.0f,	0.0f,	1.0f,	
			0.53f,     0.1f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.49f,     0.3f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.53f,     0.1f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.6f,      0.3f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.49f,     0.3f,		0.0f,		0.0f,	0.0f,	1.0f,

			0.67f,     0.1f,	    0.0f,		0.0f,	0.0f,	1.0f,
			0.6f,      0.3f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.71f,     0.3f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.67f,     0.1f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.77f,     0.1f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.71f,     0.3f,		0.0f,		0.0f,	0.0f,	1.0f,
		//Zona superior
			0.49f,     0.3f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.6f,      0.3f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.52f,     0.4f,		0.0f,		0.0f,	0.0f,	1.0f,

			0.6f,      0.3f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.52f,     0.4f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.68f,     0.4f,		0.0f,		0.0f,	0.0f,	1.0f,
					
			0.6f,      0.3f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.68f,     0.4f,		0.0f,		0.0f,	0.0f,	1.0f,
			0.71f,     0.3f,		0.0f,		0.0f,	0.0f,	1.0f
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,576);
	meshColorList.push_back(letras);
	//-------------------------------TRIÁNGULOS------------------------
	//TRIÁNGULO ROJO -- Indice 1
	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		 1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		 0.0f,	 1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
	};
	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);

	//TRIÁNGULO AMARILLO -- índice 2
	GLfloat vertices_trianguloamarillo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	1.0f,	0.0f,
		 1.0f,	-1.0f,		0.5f,			1.0f,	1.0f,	0.0f,
		 0.0f,	 1.0f,		0.5f,			1.0f,	1.0f,	0.0f,

	};
	MeshColor* trianguloamarillo = new MeshColor();
	trianguloamarillo->CreateMeshColor(vertices_trianguloamarillo, 18);
	meshColorList.push_back(trianguloamarillo);

	//TRIÁNGULO VERDE -- indice 3
	GLfloat vertices_trianguloverde[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	1.0f,	0.0f,
		 1.0f,	-1.0f,		0.5f,			0.0f,	1.0f,	0.0f,
		 0.0f,	 1.0f,		0.5f,			0.0f,	1.0f,	0.0f,

	};
	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);

	//TRIÁNGULO MAGENTA -- indice 4
	GLfloat vertices_triangulomagenta[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	1.0f,
		 1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	1.0f,
		 0.0f,	 1.0f,		0.5f,			1.0f,	0.0f,	1.0f,

	};
	MeshColor* triangulomagenta = new MeshColor();
	triangulomagenta->CreateMeshColor(vertices_triangulomagenta, 18);
	meshColorList.push_back(triangulomagenta);
	//--------------------------------CUADRADOS-------------------------
	//CUADRADO CAFÉ -- indice 5
	GLfloat vertices_cuadradocafe[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		 0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		 0.5f,	 0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		-0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		 0.5f,	 0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		-0.5f,	 0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
	};
	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe);

	//CUADRO AZUL -- indice 6
	GLfloat vertices_cuadradoazul[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		 0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		 0.5f,	 0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		 0.5f,	 0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		-0.5f,	 0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
	};
	MeshColor* cuadradoazul = new MeshColor();
	cuadradoazul->CreateMeshColor(vertices_cuadradoazul, 36);
	meshColorList.push_back(cuadradoazul);

	//CUADRO NEGRO -- indice 7 
	GLfloat vertices_cuadradonegro[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	0.0f,
		 0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	0.0f,
		 0.5f,	 0.5f,		0.5f,			0.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	0.0f,
		 0.5f,	 0.5f,		0.5f,			0.0f,	0.0f,	0.0f,
		-0.5f,	 0.5f,		0.5f,			0.0f,	0.0f,	0.0f,
	};
	MeshColor* cuadradonegro = new MeshColor();
	cuadradonegro->CreateMeshColor(vertices_cuadradonegro, 36);
	meshColorList.push_back(cuadradonegro);
}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	Shader* shaderAmarillo = new Shader();
	shaderAmarillo->CreateFromFiles(vShaderAmarillo, fShaderAmarillo);
	shaderList.push_back(*shaderAmarillo); // Índice 2

	Shader* shaderAzul = new Shader();
	shaderAzul->CreateFromFiles(vShaderAzul, fShaderAzul);
	shaderList.push_back(*shaderAzul); // Índice 3

	Shader* shaderCafe = new Shader();
	shaderCafe->CreateFromFiles(vShaderCafe, fShaderCafe);
	shaderList.push_back(*shaderCafe); // Índice 4

	Shader* shaderRojo = new Shader();
	shaderRojo->CreateFromFiles(vShaderRojo, fShaderRojo);
	shaderList.push_back(*shaderRojo); // Índice 5

	Shader* shaderVerde = new Shader();
	shaderVerde->CreateFromFiles(vShaderVerde, fShaderVerde);
	shaderList.push_back(*shaderVerde); // Índice 6

	Shader* shaderNegro = new Shader();
	shaderNegro->CreateFromFiles(vShaderNegro, fShaderNegro);
	shaderList.push_back(*shaderNegro); // Índice 7

	Shader* shaderMorado = new Shader();
	shaderMorado->CreateFromFiles(vShaderMorado, fShaderMorado);
	shaderList.push_back(*shaderMorado); // Índice 8
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	CreaPiramide();				 //índice 0 en MeshList
	CrearCubo();				 //índice 1 en MeshList
	CrearPiramideCuadrangular(); //índice 2 en MeshList
	CrearPiramideOblicua();		 //Índice 3 en MeshList
	CrearLetrasyFiguras();		 //usa MeshColor, índices en MeshColorList
	CreateShaders();

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;

	//-------Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva-------
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -10.0f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//-------Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.-------
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(1.0f,1.0f,1.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
		
		//Para el cubo y las pirámides se usa el primer set de shaders con índice 0 en ShaderList

		//ACTIVIDAD 2. PRÁCTICA 2. 
		//Base negra
		shaderList[7].useShader();
		uniformModel = shaderList[7].getModelLocation();
		uniformProjection = shaderList[7].getProjectLocation();
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.95f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 0.1f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		//--------------------------------------
		//TORRE IZQUIERDA
		//Pared izquierda y derecha.
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.9f, -0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 1.5f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		//--
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.5f, -0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 1.5f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		//Triángulos
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.7f, -0.7f, -0.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f,  0.4f, 0.4f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		// 
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.7f, -0.3f, -0.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.4f, 0.4f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		//
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.7f, 0.1f, -0.4f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.4f, 0.4f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		//------------------------------------------------------------------------------
		//FIGURA DE EN MEDIO - CUBO
		//Rombo azul
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.65f, 0.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.4f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh(); 
		//Rombo café
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.65f, 0.2f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.3f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		//Triángulo morado
		shaderList[8].useShader();
		uniformModel = shaderList[8].getModelLocation();
		uniformProjection = shaderList[8].getProjectLocation();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.125f, -0.775f, 0.0f));
		model = glm::rotate(model, -90 * toRadians,  glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[3]->RenderMesh();

		//Triángulo amarillo
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.125f, -0.515f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[3]->RenderMesh();

		//Triángulo rojo
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.125f, -0.515f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[3]->RenderMesh();

		//Triángulo verde
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.125f, -0.775f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[3]->RenderMesh();

		//-----------TERCERA FIGURA (TRIFUERZA)--------------------
				//Triángulo verde
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.5f, -0.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.3f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();
		//Triángulo ROJO
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.7f, -0.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.3f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();
		//TRIÁNGULO MORADO
		shaderList[8].useShader();
		uniformModel = shaderList[8].getModelLocation();
		uniformProjection = shaderList[8].getProjectLocation();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.6f, -0.45f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.3f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();
		//TRIÁNGULO AMARILLO
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.6f, -0.75f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.3f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[2]->RenderMesh();


		//-------------------------------------------------
		//shaderList[2].useShader(); 
		//uniformModel = shaderList[0].getModelLocation();
		//uniformProjection = shaderList[0].getProjectLocation();
		//angulo += 0.01;
		////Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		////
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshList[0]->RenderMesh();
		//----------------------------------------------------

		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		////Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		////==================================AQUÍ SE IMPRIMEN LAS LETRAS===================
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		////
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[0]->RenderMeshColor();
	

		//Ejercicio de clase:
				//-------------TRIANGULO ROJO-----------
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-0.25f, 0.5f, -0.4f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		//
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[1]->RenderMeshColor();
		////-------------TRIANGULO AMARILLO-----------
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.25f, 0.5f, -0.4f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		//
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[2]->RenderMeshColor();
		////-------------TRIANGULO VERDE-----------
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.75f, 0.5f, -0.4f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		//
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[3]->RenderMeshColor();
		////-------------TRIANGULO MAGENTA-----------
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-0.75f, 0.5f, -0.4f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		//
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[4]->RenderMeshColor();
		////-------------CUADRADO CAFÉ-----------
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-0.75f, -0.5f, -0.4f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.5f, 0.5f));
		//
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[5]->RenderMeshColor();
		////-------------CUADRADO AZUL-----------
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, -0.5f, -0.4f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.5f, 0.5f));
		//
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[6]->RenderMeshColor();
		////-------------CUADRADO NEGRO-----------
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.75f, -0.5f, -0.4f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.5f, 0.5f));
		//
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[7]->RenderMeshColor();


		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/