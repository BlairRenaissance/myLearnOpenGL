#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define GLEW_STATIC
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

class Shader{
public:
	Shader(const char *vertexPath, const char* fragmentPath);
	void use();

	ifstream vertexFile; //dist -> File Buffer
	ifstream fragmentFile; //File Buffer

	stringstream vertexSStream; //File Buffer -> String Buffer
	stringstream fragmentSStream; //String Buffer

	string vertexString;
	string fragmentString;

	const char* vertexSource;
	const char* fragmentSource;

	unsigned int ID; //Shader Program ID

private:
	void checkCompileErrors(unsigned int ID, string type);

};

