#version 450 core
layout (location = 0) in vec3 inputVertexPosition;
layout (location = 1) in vec3 inputVertexColor;
layout (location = 3) in vec3 inputVertexNormal;


out vec3 vertexColor;

uniform mat4 view;
uniform mat4 projection;


void main()
{
	gl_Position = projection * view  * vec4(inputVertexPosition, 1.f);
	vertexColor = inputVertexColor;
}
