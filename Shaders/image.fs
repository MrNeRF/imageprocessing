#version 450 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 textureCoordinates;

// texture samplers
uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, textureCoordinates);
}
