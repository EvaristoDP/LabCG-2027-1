/*
Práctica 5: Optimización y Carga de Modelos
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
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;		   //solo recibe xyz
std::vector<MeshColor*> meshListColor; // recibe xyz rgb
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;

Camera camera;
//Lista de Modelos a importar
Model Rover_M;
Model Cuerpo_M;
Model Brazo_M;
Model Pata_FD_M;
Model Pata_FI_M;
Model Pata_MD_M;
Model Pata_MI_M; 
Model Pata_TD_M; 
Model Pata_TI_M; 

Model cuerpoH;
Model vertice1;
Model vertice2;
Model vertice3;
Model vertice4;
Model vertice5;
Model vertice6;
Model vertice7;
Model vertice8;

Model cuerpoS;
Model alaSup;
Model alaIzq;
Model alaDer;

//Lista de Skybox a crear
Skybox skybox;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	 x       y       z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			 0.0f, -1.0f,  1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			 0.0f,  1.0f,  0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	 0.0f,  0.0f,	0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f, -10.0f,	10.0f,  0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f,  10.0f,	 0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f,  10.0f,	10.0f, 10.0f,	0.0f, -1.0f, 0.0f
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
}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);
	//Cargar modelos
	Rover_M = Model();
	Rover_M.LoadModel("Models/RoverModificado.obj");

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


	cuerpoH = Model();
	cuerpoH.LoadModel("Models/cuerpoH.obj");

	vertice1 = Model();
	vertice1.LoadModel("Models/vertice1.obj");
	vertice2 = Model();
	vertice2.LoadModel("Models/vertice2.obj");
	vertice3 = Model();
	vertice3.LoadModel("Models/vertice3.obj");
	vertice4 = Model();
	vertice4.LoadModel("Models/vertice4.obj");
	vertice5 = Model();
	vertice5.LoadModel("Models/vertice5.obj");
	vertice6 = Model();
	vertice6.LoadModel("Models/vertice6.obj");
	vertice7 = Model();
	vertice7.LoadModel("Models/vertice7.obj");
	vertice8 = Model();
	vertice8.LoadModel("Models/vertice8.obj");


	cuerpoS = Model();
	cuerpoS.LoadModel("Models/cuerpoS.obj");
	alaSup = Model();
	alaSup.LoadModel("Models/alaSup.obj");
	alaIzq = Model();
	alaIzq.LoadModel("Models/alaIzq.obj");
	alaDer = Model();
	alaDer.LoadModel("Models/alaDer.obj");


	//Crear Skybox con sus 6 texturas
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

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
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.5f, 0.5f, 0.5f); //piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -6.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshListModel[2]->RenderMeshModel();

		//------------*INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS-------------------*
		////Modelo Inicial
		//color = glm::vec3(0.0f, 0.0f, 1.0f); //modelo de color azul
		//
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, -2.0f, -1.5f));
		////modelaux = model;
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Rover_M.RenderModel();//modificar por el modelo de solo cuerpo del Rover, para que se pueda separar el brazo y las llantas
		//color = glm::vec3(0.0f, 0.0f, 1.0f);
		////En sesión se separara una parte del modelo y se unirá por jeraquía al cuerpo

		// MATRIZ PADRE: CUERPO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, -2.0f, 3.5f));
		modelaux = model; //Se guarda el centro del cuerpo.

		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cuerpo_M.RenderModel();

		
		//Brazo
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.9f, 2.2f, -1.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 1.0f, 0.0f));

		color = glm::vec3(0.5f, 1.0f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Brazo_M.RenderModel();


		//Siguientes modelos
		/* Ejercicio:
		1.- Separar las llantas
		2.- Hacer que al presionar una tecla cada pata de rueda pueda rotar un máximo de 45° "hacia adelante y hacia atrás"
		*/

		//	Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.5f, -0.5f, 2.0f));

		float rotFD = mainWindow.getarticulacion4();
		if (rotFD > 45.0f) rotFD = 45.0f;
		if (rotFD < -45.0f) rotFD = -45.0f;
		model = glm::rotate(model, glm::radians(rotFD), glm::vec3(0.0f, 0.0f, 1.0f));

		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pata_FD_M.RenderModel();

		//	Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.5f, -0.5f, -2.0f));

		float rotFI = mainWindow.getarticulacion5();
		if (rotFI > 45.0f) rotFI = 45.0f;
		if (rotFI < -45.0f) rotFI = -45.0f;
		model = glm::rotate(model, glm::radians(rotFI), glm::vec3(0.0f, 0.0f, 1.0f));

		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pata_FI_M.RenderModel();

		//Llanta media derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.5f, -0.5f, 2.0f));

		float rotMD = mainWindow.getarticulacion6();
		if (rotMD > 45.0f) rotMD = 45.0f;
		if (rotMD < -45.0f) rotMD = -45.0f;
		model = glm::rotate(model, glm::radians(rotMD), glm::vec3(0.0f, 0.0f, 1.0f));

		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pata_MD_M.RenderModel();
		
		// Llanta media izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.5f, -0.5f, -2.0f));

		float rotMI = mainWindow.getarticulacion7();
		if (rotMI > 45.0f) rotMI = 45.0f;
		if (rotMI < -45.0f) rotMI = -45.0f;
		model = glm::rotate(model, glm::radians(rotMI), glm::vec3(0.0f, 0.0f, 1.0f));

		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pata_MI_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.5f, -0.5f, 2.0f));

		float rotTD = mainWindow.getarticulacion8();
		if (rotTD > 45.0f) rotTD = 45.0f;
		if (rotTD < -45.0f) rotTD = -45.0f;
		model = glm::rotate(model, glm::radians(rotTD), glm::vec3(0.0f, 0.0f, 1.0f));

		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pata_TD_M.RenderModel();

		//	Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.5f, -0.5f, -2.0f));

		float rotTI = mainWindow.getarticulacion9();
		if (rotTI > 45.0f) rotTI = 45.0f;
		if (rotTI < -45.0f) rotTI = -45.0f;
		model = glm::rotate(model, glm::radians(rotTI), glm::vec3(0.0f, 0.0f, 1.0f));

		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pata_TI_M.RenderModel();

		//Holocrón
		//Cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		modelaux = model; //Se guarda el centro del cuerpo.

		color = glm::vec3(0.2f, 0.2f, 0.2f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuerpoH.RenderModel();

		//Vertice1
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.4016f, 3.8141f, 3.9431f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 1.0f, 0.0f));

		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vertice1.RenderModel();
		//Vértice2
		model = modelaux;
		model = glm::translate(model, glm::vec3(-4.03f, 3.54f, 4.1431f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 1.0f, 0.0f));

		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vertice2.RenderModel();
		//Vértice3
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.58f, -4.18f, 3.8f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(1.0f, 1.0f, 0.0f));

		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vertice3.RenderModel();
		//Vértice4
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.64f, -3.78f, 4.22f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(1.0f, 1.0f, 0.0f));

		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vertice4.RenderModel();
		//Vértice5
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.49f, 3.9f, -3.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(1.0f, 1.0f, 0.0f));

		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vertice5.RenderModel();
		//Vértice6
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.81f, 3.89f, -3.82f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(1.0f, 1.0f, 0.0f));

		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vertice6.RenderModel();
		//Vértice7
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.63f, -3.71f, -3.73f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(1.0f, 1.0f, 0.0f));

		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vertice7.RenderModel();
		//Vértice8
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.82f, -3.88f, -3.67f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(1.0f, 1.0f, 0.0f));

		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vertice8.RenderModel();
		
		//Satélite
		//Cuerpo
		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		modelaux = model; //Se guarda el centro del cuerpo.

		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuerpoS.RenderModel();

		//ala superior
		model = modelaux;
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f));

		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		alaSup.RenderModel();

		//ala Izquierda
		model = modelaux;
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));

		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		alaIzq.RenderModel();

		//ala derecha
		model = modelaux;
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));

		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		alaDer.RenderModel();



		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
