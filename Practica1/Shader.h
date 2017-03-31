#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>

class Shader {
public:
	GLuint Program;
	Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void use();
};