#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"


float vertices[] = {
	//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 0右上
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 1右下
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 2左下
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 3左上
};

unsigned int indices[] = {
	0, 2, 1,
	0, 3, 2
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
	glGenBuffers(1, &VBO); //产生buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑上
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 填进数据

	unsigned int EBO;
	glGenBuffers(1, &EBO); //产生buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // 第一个参数对应vertexSource中的location
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)) );
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //GLint size = 2
	glEnableVertexAttribArray(2);


	unsigned int texBuffer;
	glGenTextures(1, &texBuffer);
	glBindTexture(GL_TEXTURE_2D, texBuffer);
	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0); // 记得&
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		printf("Load image failed.");
	}
	stbi_image_free(data);


	unsigned int texSmile;
	glGenTextures(1, &texSmile);
	glBindTexture(GL_TEXTURE_2D, texSmile);  
    //stbi_set_flip_vertically_on_load(true); //在图片第一次被呼叫前上下颠倒 //在vertexShader中通过颠倒vertexSource中图像的y值可达到相同效果
	unsigned char* data2 = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data2) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2); //png图像的通道为RGBA
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		printf("Load image failed.");
	}
	stbi_image_free(data2);


	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0); // 纹理单元GL_TEXTURE0默认总是被激活，所以仅有一张图时无需激活任何纹理单元。
		glBindTexture(GL_TEXTURE_2D, texBuffer);
		glActiveTexture(GL_TEXTURE1); // 在绑定纹理之前先激活纹理单元GL_TEXTURE1，总共可有16个纹理单元
		glBindTexture(GL_TEXTURE_2D, texSmile);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glm::mat4 trans; //自动初始化为单位矩阵，如果初始化放在窗口刷新之外会得到超级旋转图（大概是乘旋转矩阵的速度为窗口刷新速度吧）
		// V = trans * V = (Mt(平移) * Mr(旋转) * Ms(缩放)) * V;
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		myShader->use();
		glUniform1i(glGetUniformLocation(myShader->ID, "ourTexture"), 0); //取纹理单元0
		glUniform1i(glGetUniformLocation(myShader->ID, "ourFace"), 1); //参数2为激活的纹理单元对应的索引，取纹理单元1
		glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans)); //取变换矩阵
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}