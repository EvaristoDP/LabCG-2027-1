/*
Práctica 8: Iluminación 2 
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
// Añadir Texturas del dado y del holocron
Texture dadoTexture;
Texture holocronTexture;
//Añadir Texturas de la Nave espacial y el fuego
Texture naveTexture;
Texture fuegoTexture;


Model Blackhawk_M;
//Sustituir por sus propios Rovers Con jerarquía
Model Cuerpo_M;
Model Brazo_M;
Model Pata_FD_M;
Model Pata_FI_M;
Model Pata_MD_M;
Model Pata_MI_M;
Model Pata_TD_M;
Model Pata_TI_M;

//Añadir modelo del avión.
Model Avion_M;

//Añadir modelo de la Nave
Model Nave_M;
Model Motor1_M;
Model Motor2_M;


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	
	MeshModel *obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel *obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel *obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);

	MeshModel *obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshListModel.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};
	// average normals
	GLfloat cubo_vertices[] = {
		// Front Rebels Phoenix
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.0f,   0.0f,		0.0f,	0.0f,	-1.0f,	//0
		 0.5f, -0.5f,  0.5f,	0.333f, 0.0f,		0.0f,	0.0f,	-1.0f,	//1
		 0.5f,  0.5f,  0.5f,	0.333f, 0.5f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.0f,   0.5f,		0.0f,	0.0f,	-1.0f,	//3

		// Right Republic Credits 
		//x		y		z		S		T	
		 0.5f, -0.5f,  0.5f,    0.333f, 0.0f,		-1.0f,	0.0f,	0.0f,
		 0.5f, -0.5f, -0.5f,	0.666f, 0.0f,		-1.0f,	0.0f,	0.0f,
		 0.5f,  0.5f, -0.5f,	0.666f, 0.5f,		-1.0f,	0.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,    0.333f, 0.5f,		-1.0f,	0.0f,	0.0f,

		 // Back Boba Fett Crest
		 //x		y		z		S		T		
		 -0.5f, -0.5f, -0.5f,	0.666f, 0.0f,		0.0f,	0.0f,	1.0f,
		  0.5f, -0.5f, -0.5f,	1.0f,   0.0f,		0.0f,	0.0f,	1.0f,
		  0.5f,  0.5f, -0.5f,	1.0f,   0.5f,		0.0f,	0.0f,	1.0f,
		 -0.5f,  0.5f, -0.5f,	0.666f, 0.5f,		0.0f,	0.0f,	1.0f,

		 // Left Lord Revan
		 //x		y		z		S		T
		 -0.5f, -0.5f, -0.5f,	0.0f,   0.5f,		1.0f,	0.0f,	0.0f,
		 -0.5f, -0.5f,  0.5f,	0.333f, 0.5f,		1.0f,	0.0f,	0.0f,
		 -0.5f,  0.5f,  0.5f,	0.333f, 1.0f,		1.0f,	0.0f,	0.0f,
		 -0.5f,  0.5f, -0.5f,	0.0f,   1.0f,		1.0f,	0.0f,	0.0f,

		 // Bottom Mandalorian Crest 
		 //x		y		z		S		T					
		 -0.5f, -0.5f,  0.5f,	0.333f, 0.5f,		0.0f,	1.0f,	0.0f,
		  0.5f, -0.5f,  0.5f,	0.666f, 0.5f,		0.0f,	1.0f,	0.0f,
		  0.5f, -0.5f, -0.5f,	0.666f, 1.0f,		0.0f,	1.0f,	0.0f,
		 -0.5f, -0.5f, -0.5f,	0.333f, 1.0f,		0.0f,	1.0f,	0.0f,

		 // UP Black Sun 
		 //x		y		z		S		T							
		 -0.5f,  0.5f,  0.5f,	0.666f, 0.5f,		0.0f,   -1.0f,	0.0f,
		  0.5f,  0.5f,  0.5f,	1.0f,   0.5f,		0.0f,   -1.0f,	0.0f,
		  0.5f,  0.5f, -0.5f,	1.0f,   1.0f,		0.0f,   -1.0f,	0.0f,
		 -0.5f,  0.5f, -0.5f,	0.666f, 1.0f,		0.0f,   -1.0f,	0.0f,
	};

	MeshModel* dado = new MeshModel();
	dado->CreateMeshModel(cubo_vertices, cubo_indices, 192, 36);
	meshListModel.push_back(dado);
}


void CrearHolocron()
{
	unsigned int holocron_indices[] = {

	12, 1, 0,
	 12, 4, 15,
	 14, 9, 2,
	 2, 12, 15,
	 8,  3,  4,
	 15,  4,  3,
	 3, 6, 14,
	 4, 5,  8,
	 6, 7, 13,
	 9, 10, 11,
	 11, 2, 9,
	 10, 7, 5,
	 13, 14, 6,
	 2, 3, 14,
	 2, 15, 3,
	 11, 12, 2,
	 13, 9, 14,
	 8, 6, 3,
	 12, 0, 4,
	 12, 11, 1,
	 4, 0, 5,
	 6, 8, 7,
	 9, 13, 10,
	 5, 0, 1,
	 1, 11, 10,
	 10, 13, 7,
	 7, 8, 5,
	 5, 1, 10

	};

	GLfloat holocron_vertices[] = {
		// front
		//x					y			z			S		T		NX		NY		NZ
		 -2.517274,		-2.579095,		0.002115,	0.02f,	0.00f,	0.0f,	0.0f,	1.0f,	//0
		-1.185055,		-2.530499,		-1.198261,	0.27f,	0.01f,	0.0f,	0.0f,	1.0f,	//1
		 0.049649,		 2.440128,		-2.417217,	0.50f,	0.98f,	0.0f,	0.0f,	1.0f,	//2
		 0.051152,		 2.399781,		 2.545043,	0.50f,	0.97f,	0.0f,	0.0f,	1.0f,	//3
		-2.646786,		-0.139047,		 2.493757,	0.00f,	0.48f,	0.0f,	0.0f,	1.0f,	//4
		-1.328995,		-2.578386,		 1.297838,	0.25f,	0.00f,	0.0f,	0.0f,	1.0f,	//5
		 2.593003,		-0.085222,		 2.505755,	0.98f,	0.49f,	0.0f,	0.0f,	1.0f,	//6
		 1.385028,		-2.549476,		 1.308677,	0.76f,	0.01f,	0.0f,	0.0f,	1.0f,	//7
		-0.140696,		-2.601934,		 2.558561,	0.47f,	0.00f,	0.0f,	0.0f,	1.0f,	//8
		 2.534944,		-0.061146,		-2.580280,	0.97f,	0.49f,	0.0f,	0.0f,	1.0f,	//9
		 1.344182,		-2.542571,		-1.294387,	0.75f,	0.01f,	0.0f,	0.0f,	1.0f,	//10
		 0.081432,		-2.513204,		-2.426005,	0.51f,	0.01f,	0.0f,	0.0f,	1.0f,	//11
		-2.453980,		-0.063388,		-2.477451,	0.03f,	0.49f,	0.0f,	0.0f,	1.0f,	//12
		 2.632201,		-2.553892,		 0.007226,	0.99f,	0.00f,	0.0f,	0.0f,	1.0f,	//13
		 2.521206,		 2.448241,		 0.063388,	0.97f,	0.98f,	0.0f,	0.0f,	1.0f,	//14
		-2.451372,		 2.421759,		 0.077538,	0.03f,	0.98f,	0.0f,	0.0f,	1.0f,	//15
	};

	MeshModel* holocron = new MeshModel();
	holocron->CreateMeshModel(holocron_vertices, holocron_indices, 128, 84);
	meshListModel.push_back(holocron);

}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
	CrearHolocron();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	dadoTexture = Texture("Textures/ejercicio7.jpg");
	dadoTexture.LoadTextureA();
	holocronTexture = Texture("Textures/ejercicio7.jpg");
	holocronTexture.LoadTextureA();

	//Cargar texturas de la nave
	naveTexture = Texture("Textures/textura_nave.jpg");
	naveTexture.LoadTextureA();
	fuegoTexture = Texture("Textures/textura_fuego.jpg");
	fuegoTexture.LoadTextureA();

	//Blackhawk_M = Model();
	//Blackhawk_M.LoadModel("Models/uh60.obj");
	
	//Rover_M = Model();
	//Rover_M.LoadModel("Models/rovermodificado.obj");
	//Añadir modelo del Rover
	Cuerpo_M = Model();
	Cuerpo_M.LoadModel("Models/Cuerpo.obj");

	Brazo_M = Model();
	Brazo_M.LoadModel("Models/Brazo.obj");

	Pata_FD_M = Model();
	Pata_FD_M.LoadModel("Models/Pata_FD.obj");

	Pata_FI_M = Model();
	Pata_FI_M.LoadModel("Models/Pata_FI.obj");

	Pata_MD_M = Model();
	Pata_MD_M.LoadModel("Models/Pata_MD.obj");

	Pata_MI_M = Model();
	Pata_MI_M.LoadModel("Models/Pata_MI.obj");

	Pata_TD_M = Model();
	Pata_TD_M.LoadModel("Models/Pata_TD.obj");

	Pata_TI_M = Model();
	Pata_TI_M.LoadModel("Models/Pata_TI.obj");

	
	//Añadir modelo del avión
	Avion_M = Model();
	Avion_M.LoadModel("Models/Avion.obj");

	//Añadir modelo de la nave
	Nave_M = Model();
	Nave_M.LoadModel("Models/Nave.obj");
	Motor1_M = Model();
	Motor1_M.LoadModel("Models/motor1.obj");
	Motor2_M = Model();
	Motor2_M.LoadModel("Models/motor2.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	////Declaración de la luz puntual para el Holocrón
	//pointLights[1] = PointLight(0.0f, 0.0f, 1.0f,
	//	 0.8f, 1.0f,                                
	//	-3.0f, 1.0f, 0.0f,                        
	//	 0.3f, 0.2f, 0.1f);                        
	//pointLightCount++;

	//Declaración de la luz puntual para la pinza del Rover
	pointLights[2] = PointLight(0.0f, 1.0f, 0.0f, 
		0.8f, 1.0f,                                
		26.5f, 10.0f, 2.5f,                          
		0.3f, 0.2f, 0.1f);                         
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, -15.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;
	
	//se crean mas luces puntuales y spotligh
	// Luz puntual de la nave
	PointLight luzNave = PointLight(0.0f, 1.0f, 0.0f,
		0.8f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	// Spotlight de la nave - faro delantero
	SpotLight luzFaroNave = SpotLight(1.0f, 0.0f, 0.0f,
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		20.0f);

	// Luces azules para los motores
	PointLight luzMotor1 = PointLight(0.0f, 0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	PointLight luzMotor2 = PointLight(0.0f, 0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f,0.0f, 0.0f,
		0.3f, 0.2f, 0.1f);


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	

	//COLORES PARA EL REFLECTOR DEL AVION
	glm::vec3 coloresReflector[6] =
	{
		glm::vec3(1.0f, 0.0f, 0.0f),	//Rojo
		glm::vec3(1.0f, 1.0f, 0.0f),	//Amarillo
		glm::vec3(0.0f, 1.0f, 0.0f),	//Verde
		glm::vec3(0.0f, 1.0f, 1.0f),	//Cian
		glm::vec3(0.0f, 0.0f, 1.0f),	//Azul
		glm::vec3(1.0f, 0.0f, 1.0f)		//Magenta
	};


	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());


		////Añadir el encendido y apagado de la luz puntual del Holocrón
		//if (mainWindow.getLuzHolocron()) {
		//	pointLights[1] = PointLight(0.0f, 0.0f, 1.0f,
		//		0.8f, 1.0f,
		//		-3.0f, 1.0f, 0.0f,
		//		0.3f, 0.2f, 0.1f);
		//}
		//else {
		//	pointLights[1] = PointLight(0.0f, 0.0f, 0.0f,
		//		0.0f, 0.0f,
		//		-3.0f, 1.0f, 0.0f,
		//		0.3f, 0.2f, 0.1f);
		//}

		////Añadir el encendido y apagado de la luz puntual del Rover
		//if (mainWindow.getLuzRover()) {
		//	pointLights[2] = PointLight(0.0f, 1.0f, 0.0f,
		//		0.8f, 1.0f,
		//	    26.5f, 10.0f, 2.5f,
		//		0.3f, 0.2f, 0.1f);
		//}
		//else {
		//	pointLights[2] = PointLight(0.0f, 0.0f, 0.0f,
		//		0.0f, 0.0f,
		//	    26.5f, 10.0f, 2.5f,
		//		0.3f, 0.2f, 0.1f);
		//}

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[2]->RenderMeshModel();
	
		//BLACKHAWK
		/*model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();*/

		////------------------------------------------------------------------------------------
		////Rover: Sustituir por su propio Rover con jerarquía
		//// MATRIZ PADRE: CUERPO
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(20.0f, 2.6f, 3.5f));
		//modelaux = model; //Se guarda el centro del cuerpo.

		//color = glm::vec3(0.0f, 0.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Cuerpo_M.RenderModel();


		////Brazo
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(2.9f, 2.2f, -1.1f));

		//color = glm::vec3(0.5f, 1.0f, 0.5f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Brazo_M.RenderModel();

		////	Llanta delantera derecha
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(3.5f, -0.5f, 2.0f));

		//color = glm::vec3(0.0f, 1.0f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Pata_FD_M.RenderModel();

		////	Llanta delantera izquierda
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(3.5f, -0.5f, -2.0f));

		//color = glm::vec3(0.0f, 1.0f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Pata_FI_M.RenderModel();

		////Llanta media derecha
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(-1.5f, -0.5f, 2.0f));

		//color = glm::vec3(1.0f, 1.0f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Pata_MD_M.RenderModel();

		//// Llanta media izquierda
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(-1.5f, -0.5f, -2.0f));

		//color = glm::vec3(1.0f, 1.0f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Pata_MI_M.RenderModel();

		////Llanta trasera derecha
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(-3.5f, -0.5f, 2.0f));

		//color = glm::vec3(0.0f, 1.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Pata_TD_M.RenderModel();

		////	Llanta trasera izquierda
		//model = modelaux;
		//model = glm::translate(model, glm::vec3(-3.5f, -0.5f, -2.0f));

		//color = glm::vec3(0.0f, 1.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Pata_TI_M.RenderModel();
		////------------------------------------------------------------------------------------


		////Dado
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(3.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//dadoTexture.UseTexture();
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshListModel[4]->RenderMeshModel();

		
		
		////Holocron
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-3.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//holocronTexture.UseTexture();
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshListModel[5]->RenderMeshModel();



		//************************+
		//AVION
		//Reflector:
		//GLfloat posAvionX = mainWindow.getAvionX();
		//int colorActual = mainWindow.getColorIndex();

		//spotLights[1].SetFlash(glm::vec3(posAvionX, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));

		//spotLights[1] = SpotLight(coloresReflector[colorActual].x, coloresReflector[colorActual].y, coloresReflector[colorActual].z,
		//				1.0f, 2.0f,
		//				posAvionX, 5.0f, 0.0f,
		//				0.0f, -1.0f, 0.0f,
		//				1.0f, 0.0f, 0.0f,
		//				15.0f);	

		////Añadir modelo del avión
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(posAvionX, 15.0f, 0.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Avion_M.RenderModel();

		//*********************************************************************
		//NAVE ESPACIAL
		PointLight lucesActivasP[MAX_POINT_LIGHTS];
		unsigned int contP = 0;
		SpotLight lucesActivasS[MAX_SPOT_LIGHTS];
		unsigned int contS = 0;

		//Luces que siempre están encendidas
		lucesActivasP[contP++] = pointLights[0];
		lucesActivasS[contS++] = spotLights[0];
		lucesActivasS[contS++] = spotLights[1];

		//Botones para las luces
		if (mainWindow.getLucesNave()) {
			lucesActivasP[contP++] = luzNave;
			lucesActivasS[contS++] = luzFaroNave;
		}
		if (mainWindow.getMotor1()) {
			lucesActivasP[contP++] = luzMotor1;
		}
		if (mainWindow.getMotor2()) {
			lucesActivasP[contP++] = luzMotor2;
		}

		//Enviar luces al shader
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(lucesActivasP, contP);
		shaderList[0].SetSpotLights(lucesActivasS, contS);

		//Nave
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 15.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Nave_M.RenderModel();

		//Motor 1
		if (mainWindow.getMotor1()) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-1.014f, 15.18f, -5.41f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Motor1_M.RenderModel();
			glDisable(GL_BLEND);
		}
		//Motor 2
		if (mainWindow.getMotor2()) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(1.014f, 15.18f, -5.41f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Motor2_M.RenderModel();
			glDisable(GL_BLEND);
		}
		


		////Agave ¿qué sucede si lo renderizan antes del helicóptero?
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		//model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[3]->RenderMeshModel();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
