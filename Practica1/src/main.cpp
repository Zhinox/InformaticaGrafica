//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>

#include <string>
#include <fstream>
#include <sstream>
#include "../Shader.h"

using namespace std;
const GLint WIDTH = 800, HEIGHT = 600;
bool WIREFRAME = false;
const int triangleVertex = 3;
const int cubeVertex = 4;
const int positionBufferSize = sizeof(float) * cubeVertex * triangleVertex;
const int numIndex = 6;
const int bufferIndex = sizeof(int) * numIndex;


//Positions of triangles vertices
float trianglePos[cubeVertex][triangleVertex] =
{
	{ 0.f, -0.5f, 0.f },
	{ 0.5f, -0.5f, 0.f },
	{ 0.5f, 0.f, 0.f },
	{ 0.f, 0.f, 0.f },
};

int index[] = {
	3, 0, 2,
	0, 1, 2
};

static void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//Salir de la aplicacion cuando se pulsa la tecla escape
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	//Entrar en modo wireframe cuando se pulsa la tecla W
	if (key == GLFW_KEY_W && action == GLFW_PRESS && WIREFRAME == false) {
		WIREFRAME = true;
	}
	//Salir del modo wireframe pulsando la tecla W si el wireframe esta ya activado
	else if (key == GLFW_KEY_W && action == GLFW_PRESS && WIREFRAME == true) {
		WIREFRAME = false;
	}
}

Shader *shader;
int main() {
	//initGLFW
	//TODO
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);

	//set GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	//create a window
	//TODO
	window = glfwCreateWindow(WIDTH, HEIGHT, "Practica1_AntonioRodriguez", nullptr, nullptr);
	if (!window) {
		cout << "Error al crear ventana" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	//set GLEW and inicializate
	//TODO
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		cout << "Error al inicializar glew" << endl;
		glfwTerminate();
		return NULL;
	}

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//set function when callback
	//TODO
	glfwSetKeyCallback(window, key_callback);

	GLuint vao, vbo, ebo;

	//cargamos los shader
	shader = new Shader("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");

	glGenVertexArrays(1, &vao); //Create new VAO
	glBindVertexArray(vao); //Store connections between VBOs and attributs

	glGenBuffers(1, &vbo); //Create VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo); 
	glBufferData(GL_ARRAY_BUFFER, positionBufferSize, trianglePos, GL_STATIC_DRAW);
	// indicate that current VBO should be used with vertex attribute with index 0
	glEnableVertexAttribArray(0);
	// indicate how vertex attribute 0 should interpret data in connected VBO
	glVertexAttribPointer(0, triangleVertex, GL_FLOAT, GL_FALSE, 0, 0);

	// Create new buffer that will be used to store indices
	glGenBuffers(1, &ebo);
	// Bind index buffer to corresponding target
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	// Inicialize index buffer, allocate memory, fill it with data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferIndex, index, GL_STATIC_DRAW);

	//bucle de dibujado
	while (!glfwWindowShouldClose(window))
	{
		//origen de la camara, dimensiones de la ventana
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT);

		//color de fondo
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		//definir la matriz de proyeccion
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-10, 10, -10.f, 10.f, -1.0f, 10.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//activar el culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		//GL_CW sentido horario, GL_CCW sentido antihorario
		glFrontFace(GL_CCW);

		//shader
		shader->use();

		glBindVertexArray(vao);

		if (WIREFRAME) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		//pintar el VAO
		glDrawElements(GL_TRIANGLES, numIndex, GL_UNSIGNED_INT, NULL);

		// reset binding of VAO
		glBindVertexArray(0);

		//change of frameBuffer
		glfwSwapBuffers(window);

		//comprueba que algun disparador se haya activado (tales como el teclado, raton, etc)
		glfwPollEvents();
	}
	//reset bindings for VAO, VBO and EBO and set free also with glDeleteVertexArrays()
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteVertexArrays(1, &vao);
	glDeleteVertexArrays(1, &vbo);
	glDeleteVertexArrays(1, &ebo);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}