#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	//Inicializando variables para el avión
	avionX = 0.0f;
	colorIndex = 0;
	cTeclaPresionada = false;

	//Inicializando variables para el holocron
	luzHolocron = true;
	hTeclaPresionada = false;

	//Inicializando variables para el Rover
	luzRover = false;
	rTeclaPresionada = false;

	//Inicializando variables para la nave espacial
	lucesNave = false; 
	nTeclaPresionada = false;

	motor1 = false;  
	unoTeclaPresionada = false;
	
	motor2 = false;  
	dosTeclaPresionada = false;

	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica08:Iluminacion 2 - Ramirez del Prado Evaristo", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_Y)
	{
		theWindow-> muevex += 1.0;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow-> muevex -= 1.0;
	}

	//Añadiendo las teclas para el avion y el faro
	if (key == GLFW_KEY_L) {
		theWindow->avionX += 0.5f;
	}
	if (key == GLFW_KEY_K) {
		theWindow->avionX -= 0.5f;
	}
	if (key == GLFW_KEY_C) {
		if (action == GLFW_PRESS && !theWindow->cTeclaPresionada) {
			theWindow->colorIndex = (theWindow->colorIndex + 1) % 6;
			theWindow->cTeclaPresionada = true;
		}
		else if (action == GLFW_RELEASE) {
			theWindow->cTeclaPresionada = false;
		}
	}
	//Añadiendo las teclas para la luz del holocrón
	if (key == GLFW_KEY_H) {
		if (action == GLFW_PRESS && !theWindow->hTeclaPresionada) {
			theWindow->luzHolocron = !theWindow->luzHolocron;
			theWindow->hTeclaPresionada = true;
		}
		else if (action == GLFW_RELEASE) {
			theWindow->hTeclaPresionada = false;
		}
	}

	//Añadiendo las teclas para el Rover
	if (key == GLFW_KEY_R) {
		if (action == GLFW_PRESS && !theWindow->rTeclaPresionada) {
			theWindow->luzRover = !theWindow->luzRover;
			theWindow->rTeclaPresionada = true;
		}
		else if (action == GLFW_RELEASE) {
			theWindow->rTeclaPresionada = false;
		}
	}

	//Añadiendo las teclas para la nave espacial
	// Tecla N
	if (key == GLFW_KEY_N) {
		if (action == GLFW_PRESS && !theWindow->nTeclaPresionada) {
			theWindow->lucesNave = !theWindow->lucesNave;
			theWindow->nTeclaPresionada = true;
		}
		else if (action == GLFW_RELEASE) { theWindow->nTeclaPresionada = false; }
	}

	// Tecla 1
	if (key == GLFW_KEY_1) {
		if (action == GLFW_PRESS && !theWindow->unoTeclaPresionada) {
			theWindow->motor1 = !theWindow->motor1;
			theWindow->unoTeclaPresionada = true;
		}
		else if (action == GLFW_RELEASE) { theWindow->unoTeclaPresionada = false; }
	}

	// Tecla 2
	if (key == GLFW_KEY_2) {
		if (action == GLFW_PRESS && !theWindow->dosTeclaPresionada) {
			theWindow->motor2 = !theWindow->motor2;
			theWindow->dosTeclaPresionada = true;
		}
		else if (action == GLFW_RELEASE) { theWindow->dosTeclaPresionada = false; }
	}


	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
