#version 450 core
layout (location = 0) in vec2 inputVertexPostion;
layout (location = 1) in vec3 inputVertexColor;
layout (location = 2) in vec2 inputTextureCoordinate;

out vec3 vertexColor;
out vec2 textureCoordinates;

void main()
{
    vertexColor = inputVertexColor;
    textureCoordinates = inputTextureCoordinate;
    gl_Position = vec4(inputVertexPostion, 1.f, 1.f);
}
