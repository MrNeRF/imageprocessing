#version 420 core

out vec4 LightColor; 

in vec3 vertexColor;
  

void main()
{
    LightColor = vec4(vertexColor, 1.f);
} 
