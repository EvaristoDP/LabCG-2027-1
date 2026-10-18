#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	//Función para el ciclo de color del faro
	int getColorIndex() { return colorIndex; }
	//Función para la posición del avión
	GLfloat getAvionX() { return avionX; }
	//VFunción para luz del holocrón
	bool getLuzHolocron() { return luzHolocron; }
	//Función para la luz de la pinza del Rover
	bool getLuzRover() { return luzRover; }
	//Funciones para la luz de la nave y los motores
	bool getLucesNave() { return lucesNave; }
	bool getMotor1() { return motor1; }
	bool getMotor2() { return motor2; }


	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

	//Variables para el avión
	GLfloat avionX;
	int colorIndex;
	bool cTeclaPresionada;

	//Variables para el holocrón
	bool luzHolocron;
	bool hTeclaPresionada;

	//Variables para el Rover
	bool luzRover;
	bool rTeclaPresionada;

	//Variables para la nave y los motores
	bool lucesNave, nTeclaPresionada;
	bool motor1, unoTeclaPresionada;
	bool motor2, dosTeclaPresionada;
};

