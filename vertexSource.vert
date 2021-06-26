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
    TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y); //图片的顶点在左上角，OpenGL的顶点在左下角，需要翻转图片的顶点坐标
}