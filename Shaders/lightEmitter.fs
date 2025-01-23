#version 400 core

in vec3 ourColor;
in vec2 texCoord;

out vec4 FragColor;

void main() {    	   
	FragColor = vec4(ourColor, 1.0);
}