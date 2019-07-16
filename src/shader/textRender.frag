#version 330 core

out vec4 color;


in vec2 TexCoords;

uniform sampler2D image;
uniform vec3 textColor;

void main(){
	vec4 sampled = vec4(1.0f,1.0f,1.0f, texture(image,TexCoords).r);
	color = vec4(textColor, 1.0f) * sampled;
}
