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
	//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 0����
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 1����
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 2����
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 3����
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // OpenGL3.3�汾

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
	glGenBuffers(1, &VBO); //����buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // ����
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // �������

	unsigned int EBO;
	glGenBuffers(1, &EBO); //����buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // ��һ��������ӦvertexSource�е�location
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)) );
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //GLint size = 2
	glEnableVertexAttribArray(2);


	unsigned int texBuffer;
	glGenTextures(1, &texBuffer);
	glBindTexture(GL_TEXTURE_2D, texBuffer);
	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0); // �ǵ�&
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
    //stbi_set_flip_vertically_on_load(true); //��ͼƬ��һ�α�����ǰ���µߵ� //��vertexShader��ͨ���ߵ�vertexSource��ͼ���yֵ�ɴﵽ��ͬЧ��
	unsigned char* data2 = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data2) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2); //pngͼ���ͨ��ΪRGBA
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

		glActiveTexture(GL_TEXTURE0); // ����ԪGL_TEXTURE0Ĭ�����Ǳ�������Խ���һ��ͼʱ���輤���κ�����Ԫ��
		glBindTexture(GL_TEXTURE_2D, texBuffer);
		glActiveTexture(GL_TEXTURE1); // �ڰ�����֮ǰ�ȼ�������ԪGL_TEXTURE1���ܹ�����16������Ԫ
		glBindTexture(GL_TEXTURE_2D, texSmile);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glm::mat4 trans; //�Զ���ʼ��Ϊ��λ���������ʼ�����ڴ���ˢ��֮���õ�������תͼ������ǳ���ת������ٶ�Ϊ����ˢ���ٶȰɣ�
		// V = trans * V = (Mt(ƽ��) * Mr(��ת) * Ms(����)) * V;
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		myShader->use();
		glUniform1i(glGetUniformLocation(myShader->ID, "ourTexture"), 0); //ȡ����Ԫ0
		glUniform1i(glGetUniformLocation(myShader->ID, "ourFace"), 1); //����2Ϊ���������Ԫ��Ӧ��������ȡ����Ԫ1
		glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans)); //ȡ�任����
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}