#version 450 core
layout (location = 0) in vec2 vertexPosition;
layout (location = 2) in vec3 inputVertexColor;

out vec3 vertexColor;
void main()
{
    vertexColor = inputVertexColor;
    gl_Position = vec4(vertexPosition, 0.f, 1.f);
}
