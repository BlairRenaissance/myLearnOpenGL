#version 330 core 

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture; //�ز�Ϊsampler
uniform sampler2D ourFace;

void main(){
    FragColor = mix(texture(ourTexture, TexCoord), texture(ourFace, TexCoord), 0.2); 
	//0.2�᷵��80%�ĵ�һ��������ɫ��20%�ĵڶ���������ɫ�����������������Ļ��ɫ��
}