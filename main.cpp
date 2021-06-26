#include <iostream>
#include <vector>
#include <cmath>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"

#pragma region Model Data
float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};
#pragma endregion

const unsigned int screenWidth = 800;
const unsigned int screenHeight = 600;

#pragma region Camera Declare
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
Camera camera(glm::vec3(1.0f, 0.0f, 6.0f), glm::radians(15.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
#pragma endregion

#pragma region Input Declare
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.speedZ = -0.01f; // camera向前是向-Z方向移动
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.speedZ = 0.01f;
	}
	else {
		camera.speedZ = 0;
	}
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos) {
	if (firstMouse == true) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}
	float deltaX, deltaY;
	deltaX = xPos - lastX;
	deltaY = lastY - yPos; // 注意这里是相反的，因为屏幕+y方向向下

	lastX = xPos;
	lastY = yPos;

	camera.processMouseMovement(deltaX, deltaY);
}
#pragma endregion

unsigned int LoadImageToGPU(const char* filename, GLint internalFormat, GLint format, int textureSlot) {
	// Generate and bind the first textBuffer of the square
	unsigned int texBuffer;
	glGenTextures(1, &texBuffer);
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, texBuffer);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0); // remember to use &
	if (data)
	{
		// add data 
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Load image failed.");
	}
	stbi_image_free(data);
	return texBuffer;
}

int main(){
	#pragma region Open a Window
	glfwInit(); //Initialize GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //OpenGL3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Use core mode (disordered backward compatibility)

	//Create window (width, height, window name)
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "My OpenGL Game", NULL, NULL);
	//Check whether the window is created successfully
	if (window == NULL) {
		printf("Open window falied.");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); //Set the context of the window to the main context of the current process
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);

	//Initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		printf("Init GLEW failed.");
		glfwTerminate();
		return -1;
	}

	//Specify the current viewport size 
	//the first two parameters are the lower left corner position
	//the last two parameters are the width and height of the rendering window)
	glViewport(0, 0, screenWidth, screenHeight);

	glEnable(GL_DEPTH_TEST); //enable Z-buffer
	#pragma endregion

	#pragma region Init Shader Program
	Shader* myShader = new Shader("vertexSource.vert", "fragmentSource.frag");
	#pragma endregion

	#pragma region Init and Load Models to VAO, VBO
	// Generate and bind the VAO of the square
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate and bind the VBO of the square
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Bind the vertices data to the current default buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set the vertex attribute pointer
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // 启用location0单元
	// texture coord attribute, size = 2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2); // 启用location2单元

	// Unbind VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	#pragma endregion

	#pragma region Load Texture
	unsigned int texBuffer;
	texBuffer = LoadImageToGPU("container.jpg", GL_RGB, GL_RGB, 0);
	unsigned int texSmile;
	texSmile = LoadImageToGPU("awesomeface.png", GL_RGBA, GL_RGBA, 1);
	#pragma endregion

	#pragma region Prepare MVP matrices
	glm::mat4 modelMat;
	glm::mat4 viewMat;
	glm::mat4 projMat;
	projMat = glm::perspective(glm::radians(30.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	#pragma endregion

#pragma region 
	while (!glfwWindowShouldClose(window))
	{
		// Process Input
		processInput(window);

		// Clear Screen
		glClearColor(0.1f, 0.7f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		viewMat = camera.getViewMatrix();

		for (unsigned int i = 0; i < 10; i++){
			// Set Model matrix
			modelMat = glm::translate(glm::mat4(1.0f), cubePositions[i]);

			// Set View and Projection Matrix here if you need.
			// ToDo

			// Set Material -> Shader Program
			myShader->use();
			// Set Material -> Textures
			glActiveTexture(GL_TEXTURE0); // 纹理单元GL_TEXTURE0默认总是被激活，所以仅有一张图时无需激活任何纹理单元。
			glBindTexture(GL_TEXTURE_2D, texBuffer);
			glActiveTexture(GL_TEXTURE1); // 在绑定纹理之前先激活纹理单元GL_TEXTURE1，总共可有16个纹理单元
			glBindTexture(GL_TEXTURE_2D, texSmile);
			// Set Material -> Uniforms
			glUniform1i(glGetUniformLocation(myShader->ID, "ourTexture"), 0); //取纹理单元0
			glUniform1i(glGetUniformLocation(myShader->ID, "ourFace"), 1); //参数2为激活的纹理单元对应的索引，取纹理单元1
			glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
			glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
			glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));			

			// Set Model
			glBindVertexArray(VAO);
			
			// Drawcall
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Clean up, prepare for next render loop
		glfwSwapBuffers(window);
		glfwPollEvents();
		camera.updateCameraPos();
	}

	// Exit program
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}