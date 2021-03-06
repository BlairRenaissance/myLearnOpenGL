#version 330 core 

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture; //素材为sampler
uniform sampler2D ourFace;

void main(){
    FragColor = mix(texture(ourTexture, TexCoord), texture(ourFace, TexCoord), 0.2); 
	//0.2会返回80%的第一个输入颜色和20%的第二个输入颜色，即返回两个纹理的混合色。
}