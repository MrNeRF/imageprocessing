#version 420 core

out vec4 FragColor; 

in vec3 vertexWorldPosition;  
in vec3 vertexNormal;  
in vec3 vertexColor;
  
uniform vec3 lightPosition; 
uniform vec3 lightColor;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(vertexNormal);
    vec3 lightDir = normalize(lightPosition - vertexWorldPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
            
    vec3 result = (ambient + diffuse) * vertexColor;
    /* FragColor = vec4(result, 1.0); */
    FragColor = vec4(0.9f, 0.6f, 0.6f, 1.f);
} 
