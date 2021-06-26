#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main(){
	// ----- note that we read the multiplication from right to left -----
    gl_Position = projMat * viewMat * modelMat * vec4(aPos, 1.0);
    TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y); //ͼƬ�Ķ��������Ͻǣ�OpenGL�Ķ��������½ǣ���Ҫ��תͼƬ�Ķ�������
}