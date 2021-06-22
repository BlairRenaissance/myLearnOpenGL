#include <iostream>

#define GLEW_STATIC
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

float vertices[] = {
	-0.5f, -0.5f, 0.0f, 1.0f, 0, 0,//0
	0.5f, -0.5f, 0.0f, 0, 1.0f, 0, //1
	0.0f, 0.5f, 0.0f, 0, 0, 1.0f,  //2
	0.8f, 0.8f, 0.0f, 1.0f, 0, 1.0f   //3
};

unsigned int indices[] = {
	0, 1, 2,
	2, 1, 3
};


void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}


int main(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // OpenGL3.3版本

	// Open GLFW Window
	GLFWwindow* window = glfwCreateWindow(800, 600, "My OpenGL Game", NULL, NULL);
	if (window == NULL) {
		printf("Open window falied.");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// init GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		printf("Init GLEW failed.");
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);

	Shader* myShader = new Shader("vertexSource.txt", "fragmentSource.txt");

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//unsigned int vertexShader;
	//vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//glCompileShader(vertexShader);

	//unsigned int fragmentShader;
	//fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//glCompileShader(fragmentShader);

	//unsigned int shaderProgram;
	//shaderProgram = glCreateProgram();
	//glAttachShader(shaderProgram, vertexShader);
	//glAttachShader(shaderProgram, fragmentShader);
	//glLinkProgram(shaderProgram);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // 第一个参数对应vertexShaderSource中location
	//glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // 第一个参数对应vertexSource中的location
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)) ); // 第一个参数对应vertexSource中的location
	glEnableVertexAttribArray(1);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		//===== Use Uniform to draw timechange color =====
		//float timeValue = glfwGetTime();
		//float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		//glUseProgram(shaderProgram);
		//glUniform4f(vertexColorLocation, 0, greenValue, 0, 1.0f);

		myShader->use();

		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}