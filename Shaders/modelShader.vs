#version 420 core
layout (location = 0) in vec3 inputVertexPosition;
layout (location = 1) in vec3 inputVertexColor;
layout (location = 3) in vec3 inputVertexNormal;

out vec3 vertexWorldPosition;
out vec3 vertexColor;
out vec3 vertexNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vertexWorldPosition = vec3(model * vec4(inputVertexPosition, 1.0));
    vertexColor = inputVertexColor;
    vertexNormal = inputVertexNormal;  
    
    gl_Position = projection * view * model * vec4(inputVertexPosition, 1.f);
}
